#include "TextEditMew.h"
#include <QGraphicsSceneMouseEvent>
#include <QFocusEvent>

TextEditMew::TextEditMew(ColorScheme& scheme, QGraphicsItem* parent)
    : QGraphicsObject(parent)
    , _scheme(scheme)
{
    _suppressEnsureVisible = true;

    setFlag(ItemIsFocusable, true);

    _backgroundColor     = _scheme.backgroundGet();
    _borderColor         = _scheme.borderGet();
    _focusedBorderColor  = _scheme.additionalColorGet();
    _textColor           = _scheme.textColorGet();
    _placeholderColor    = QColor(150, 150, 150);

    _currentHeight = _minHeight;
    _bounds = QRectF(0, 0, _maxWidth, _currentHeight);

    _cursorTimer = new QTimer(this);
    connect(_cursorTimer, &QTimer::timeout,
            this, &TextEditMew::toggleCursorVisible);
    _cursorTimer->start(500);
}

QRectF TextEditMew::boundingRect() const
{
    return QRectF(0, 0, _maxWidth, _currentHeight)
    .adjusted(-_borderWidth, -_borderWidth,
              _borderWidth,  _borderWidth);
}

void TextEditMew::paint(QPainter* p,
                        const QStyleOptionGraphicsItem*,
                        QWidget*)
{
    p->setRenderHint(QPainter::Antialiasing, true);
    // if (_scrollEnabled)
    //     p->translate(0, -_scrollOffset);


    QRectF br = boundingRect();
    QRectF r = br.adjusted(
        _borderWidth / 2.0,
        _borderWidth / 2.0,
        -_borderWidth / 2.0,
        -_borderWidth / 2.0
        );

    // фон
    p->setBrush(_backgroundColor);
    p->setPen(Qt::NoPen);
    p->drawRoundedRect(r, 10, 10);

    // рамка
    p->setPen(QPen(_hasFocus ? _focusedBorderColor : _borderColor, _borderWidth));
    p->setBrush(Qt::NoBrush);
    p->drawRoundedRect(r, 10, 10);

    // область текста
    QRectF textRect = r.adjusted(
        _padding,
        _padding,
        -_padding,
        -_padding
        );

    QFont font;
    p->setFont(font);
    QFontMetricsF fm(font);

    // клип + скролл
    p->save();
    p->setClipRect(textRect);
    p->translate(0, -_scrollOffset);

    QStringList lines = _text.split('\n');
    if (lines.isEmpty())
        lines << QString();

    qreal y = textRect.top() + fm.ascent();

    if (_text.isEmpty() && !_hasFocus && !_placeholder.isEmpty())
    {
        p->setPen(_placeholderColor);
        p->drawText(QPointF(textRect.left(), y), _placeholder);
    }
    else
    {
        p->setPen(_textColor);
        for (const QString& line : lines)
        {
            p->drawText(QPointF(textRect.left(), y), line);
            y += fm.height();
        }
    }

    // курсор
    if (_hasFocus && _cursorVisible)
    {
        QPointF pt = cursorPositionToPoint(_cursorPos);
        if (pt.y() >= textRect.top() &&
            pt.y() <= textRect.bottom() + _scrollOffset)
        {
            p->setPen(QPen(_textColor, 1));
            p->drawLine(
                pt.x(),
                pt.y() - fm.ascent(),
                pt.x(),
                pt.y() + fm.descent()
                );
        }
    }

    p->restore();
}

void TextEditMew::setText(const QString& text)
{
    _text = text;
    _cursorPos = _text.size();
    updateLayout();
    emit textChanged(_text);
    update();
}

QString TextEditMew::text() const
{
    return _text;
}

void TextEditMew::setPlaceholder(const QString& text)
{
    _placeholder = text;
    update();
}

void TextEditMew::setMultiline(bool enabled)
{
    _multiline = enabled;
}

void TextEditMew::setSingleLine(bool enabled)
{
    _multiline = !enabled;
}

void TextEditMew::setAutoExpand(bool enabled)
{
    _autoExpand = enabled;
    updateLayout();
}

void TextEditMew::setMaxChars(int n)
{
    _maxChars = n;
}

void TextEditMew::setMinHeight(qreal h)
{
    _minHeight = h;
    if (_currentHeight < _minHeight)
        _currentHeight = _minHeight;
    updateLayout();
}

void TextEditMew::setMaxHeight(qreal h)
{
    _maxHeight = h;
    updateLayout();
}

void TextEditMew::setMaxWidth(qreal w)
{
    _maxWidth = w;
    updateLayout();
}

qreal TextEditMew::textHeight() const
{
    QFont font;
    QFontMetricsF fm(font);

    qreal h = 0;
    for (const auto& line : _text.split('\n'))
        h += fm.height();

    return h + 2 * _padding; // без рамки, это чисто текстовая высота
}

void TextEditMew::keyPressEvent(QKeyEvent* e)
{
    QString t = e->text();

    if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
    {
        if (_multiline)
            insertText("\n");
        else
            emit submitted(_text);
        return;
    }

    switch (e->key())
    {
    case Qt::Key_Backspace: deletePreviousChar(); return;
    case Qt::Key_Delete:    deleteNextChar();     return;
    case Qt::Key_Left:      moveCursorLeft(false); return;
    case Qt::Key_Right:     moveCursorRight(false); return;
    case Qt::Key_Home:      moveCursorToStart(); return;
    case Qt::Key_End:       moveCursorToEnd();   return;
    }

    if (!t.isEmpty())
    {
        QChar c = t[0];

        if (_maxChars > 0 && _text.size() >= _maxChars)
        {
            if (e->key() != Qt::Key_Backspace &&
                e->key() != Qt::Key_Delete &&
                e->key() != Qt::Key_Left &&
                e->key() != Qt::Key_Right &&
                e->key() != Qt::Key_Home &&
                e->key() != Qt::Key_End)
            {
                return;
            }
        }

        if (c.isPrint() || c.isSpace())
            insertText(t);
    }
}

void TextEditMew::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    setFocus(Qt::MouseFocusReason);
    _cursorPos = positionFromPoint(e->pos() + QPointF(0, _scrollOffset));
    ensureCursorVisible();
    update();
}

void TextEditMew::focusInEvent(QFocusEvent*)
{
    _hasFocus = true;
    _cursorVisible = true;
    update();
}

void TextEditMew::focusOutEvent(QFocusEvent*)
{
    _hasFocus = false;
    _cursorVisible = false;
    update();
}

void TextEditMew::wheelEvent(QGraphicsSceneWheelEvent* e)
{
    if (!_scrollEnabled) return;

    qreal fullTextH = textHeight();
    qreal visibleH  = _currentHeight - 2 * _padding;

    if (fullTextH <= visibleH)
        return;

    _scrollOffset -= e->delta() / 4.0;

    if (_scrollOffset < 0)
        _scrollOffset = 0;

    qreal maxOffset = fullTextH - visibleH;
    if (_scrollOffset > maxOffset)
        _scrollOffset = maxOffset;

    update();
}

void TextEditMew::toggleCursorVisible()
{
    if (_hasFocus)
    {
        _cursorVisible = !_cursorVisible;
        update();
    }
}

void TextEditMew::updateLayout()
{
    qreal fullTextH = textHeight();
    qreal newH = fullTextH;

    if (!_autoExpand)
        newH = _minHeight;

    if (newH < _minHeight)
        newH = _minHeight;

    if (newH > _maxHeight)
        newH = _maxHeight;

    qreal oldH = _currentHeight;

    prepareGeometryChange();
    _currentHeight = newH;
    _bounds = QRectF(0, 0, _maxWidth, _currentHeight);

    if (!qFuzzyCompare(oldH, _currentHeight))
        emit heightChanged(_currentHeight);

    ensureCursorVisible();
    emit cursorMoved(cursorPositionToPoint(_cursorPos).y());

    update();
    _suppressEnsureVisible = false;

}

void TextEditMew::insertText(const QString& t)
{
    _text.insert(_cursorPos, t);
    _cursorPos += t.size();
    updateLayout();
    emit textChanged(_text);
}

void TextEditMew::deletePreviousChar()
{
    if (_cursorPos <= 0)
        return;

    _text.remove(_cursorPos - 1, 1);
    _cursorPos--;
    updateLayout();
    emit textChanged(_text);
}

void TextEditMew::deleteNextChar()
{
    if (_cursorPos >= _text.size())
        return;

    _text.remove(_cursorPos, 1);
    updateLayout();
    emit textChanged(_text);
}

void TextEditMew::moveCursorLeft(bool)
{
    if (_cursorPos > 0)
        _cursorPos--;
    ensureCursorVisible();
    emit cursorMoved(cursorPositionToPoint(_cursorPos).y());
    update();
}

void TextEditMew::moveCursorRight(bool)
{
    if (_cursorPos < _text.size())
        _cursorPos++;
    ensureCursorVisible();
    emit cursorMoved(cursorPositionToPoint(_cursorPos).y());
    update();
}

void TextEditMew::moveCursorToEnd()
{
    _cursorPos = _text.size();
    ensureCursorVisible();
    emit cursorMoved(cursorPositionToPoint(_cursorPos).y());
    update();
}

void TextEditMew::moveCursorToStart()
{
    _cursorPos = 0;
    ensureCursorVisible();
    emit cursorMoved(cursorPositionToPoint(_cursorPos).y());
    update();
}

void TextEditMew::ensureCursorVisible()
{
    if (!_scrollEnabled) { _scrollOffset = 0; return; }

    if (_suppressEnsureVisible)
    {
        _scrollOffset = 0;
        return;
    }


    QFont font;
    QFontMetricsF fm(font);

    qreal fullTextH = textHeight();
    qreal visibleH  = _currentHeight - 2 * _padding;

    if (fullTextH <= visibleH)
    {
        _scrollOffset = 0;
        return;
    }

    QPointF pt = cursorPositionToPoint(_cursorPos);
    qreal cy = pt.y();

    qreal deadZone = fm.height(); // высота одной строки

    // вверх
    if (cy < _scrollOffset + _padding)
        _scrollOffset = cy - _padding;

    // вниз (НО с мёртвой зоной)
    if (cy > _scrollOffset + visibleH - deadZone)
        _scrollOffset = cy - (visibleH - deadZone);

    if (_scrollOffset < 0)
        _scrollOffset = 0;

    qreal maxOffset = fullTextH - visibleH;
    if (_scrollOffset > maxOffset)
        _scrollOffset = maxOffset;
}

int TextEditMew::positionFromPoint(const QPointF& p) const
{
    QFont font;
    QFontMetricsF fm(font);

    QRectF textRect = QRectF(0, 0, _maxWidth, _currentHeight)
                          .adjusted(_padding, _padding,
                                    -_padding, -_padding);

    QStringList lines = _text.split('\n');
    if (lines.isEmpty())
        lines << QString();

    qreal y = textRect.top();
    int pos = 0;

    for (const QString& line : lines)
    {
        if (p.y() >= y && p.y() <= y + fm.height())
        {
            qreal x = textRect.left();
            for (int i = 0; i <= line.size(); ++i)
            {
                if (fm.horizontalAdvance(line.left(i)) + x > p.x())
                    return pos + i;
            }
            return pos + line.size();
        }

        y += fm.height();
        pos += line.size() + 1;
    }

    return _text.size();
}

QPointF TextEditMew::cursorPositionToPoint(int pos) const
{
    QFont font;
    QFontMetricsF fm(font);

    QRectF textRect = QRectF(0, 0, _maxWidth, _currentHeight)
                          .adjusted(_padding, _padding,
                                    -_padding, -_padding);

    QStringList lines = _text.split('\n');
    if (lines.isEmpty())
        lines << QString();

    int cur = 0;
    qreal y = textRect.top() + fm.ascent();

    for (const QString& line : lines)
    {
        if (pos <= cur + line.size())
        {
            qreal w = fm.horizontalAdvance(line.left(pos - cur));
            return QPointF(textRect.left() + w, y);
        }

        cur += line.size() + 1;
        y += fm.height();
    }

    return QPointF(textRect.left(), y);
}
