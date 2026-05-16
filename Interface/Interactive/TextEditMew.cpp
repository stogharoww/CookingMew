#include "TextEditMew.h"
#include <QGraphicsSceneMouseEvent>
#include <QFocusEvent>
#include <QDebug>

TextEditMew::TextEditMew(ColorScheme& scheme, QGraphicsItem* parent)
    : QGraphicsObject(parent)
    , _scheme(scheme)
{
    setFlag(ItemIsFocusable, true);

    _backgroundColor     = _scheme.backgroundGet();
    _borderColor         = _scheme.borderGet();
    _focusedBorderColor  = _scheme.additionalColorGet();
    _textColor           = _scheme.textColorGet();
    _placeholderColor    = QColor(150, 150, 150);

    _bounds = QRectF(0, 0, _maxWidth, _minHeight);

    _cursorTimer = new QTimer(this);
    connect(_cursorTimer, &QTimer::timeout,
            this, &TextEditMew::toggleCursorVisible);
    _cursorTimer->start(500);
}

QRectF TextEditMew::boundingRect() const
{
    return _bounds.adjusted(-1, -1, 1, 1);
}

void TextEditMew::paint(QPainter* painter,
                        const QStyleOptionGraphicsItem*,
                        QWidget*)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    // фон
    painter->setBrush(_backgroundColor);
    painter->setPen(QPen(_hasFocus ? _focusedBorderColor : _borderColor, 2));
    painter->drawRoundedRect(_bounds, 10, 10);

    // текст
    QFont font;
    painter->setFont(font);
    QFontMetricsF fm(font);

    QRectF textRect = _bounds.adjusted(_padding, _padding,
                                       -_padding, -_padding);

    painter->setClipRect(textRect);

    if (_text.isEmpty() && !_hasFocus && !_placeholder.isEmpty()) {
        painter->setPen(_placeholderColor);
        painter->drawText(textRect.topLeft(), _placeholder);
    } else {
        painter->setPen(_textColor);

        QStringList lines = _text.split('\n');
        qreal y = textRect.top() + fm.ascent();

        if (lines.isEmpty())
            lines << QString();

        for (const QString& line : lines) {
            painter->drawText(QPointF(textRect.left(), y), line);
            y += fm.height();
        }
    }

    // курсор
    if (_hasFocus && _cursorVisible) {
        QPointF cursorPt = cursorPositionToPoint(_cursorPos);
        if (textRect.contains(cursorPt)) {
            painter->setPen(QPen(_textColor, 1));
            painter->drawLine(cursorPt.x(),
                              cursorPt.y() - fm.ascent(),
                              cursorPt.x(),
                              cursorPt.y() + fm.descent());
        }
    }
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

void TextEditMew::setAutoExpand(bool enabled)
{
    _autoExpand = enabled;
}

void TextEditMew::setMinHeight(qreal h)
{
    _minHeight = h;
    updateLayout();
}

void TextEditMew::setMaxWidth(qreal w)
{
    _maxWidth = w;
    updateLayout();
}

void TextEditMew::keyPressEvent(QKeyEvent* event)
{
    QString t = event->text();

    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        if (_multiline) {
            insertText("\n");
        } else {
            emit submitted(_text);
        }
        return;
    }

    switch (event->key()) {
    case Qt::Key_Backspace:
        deletePreviousChar();
        return;
    case Qt::Key_Delete:
        deleteNextChar();
        return;
    case Qt::Key_Left:
        moveCursorLeft(false);
        return;
    case Qt::Key_Right:
        moveCursorRight(false);
        return;
    case Qt::Key_Home:
        moveCursorToStart();
        return;
    case Qt::Key_End:
        moveCursorToEnd();
        return;
    default:
        break;
    }

    if (!t.isEmpty()) {
        QChar c = t[0];
        if (c.isPrint() || c.isSpace()) {
            insertText(t);
        }
    }
}

void TextEditMew::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    setFocus(Qt::MouseFocusReason);
    QPointF p = event->pos();
    _cursorPos = positionFromPoint(p);
    ensureCursorVisible();
    update();
}

void TextEditMew::focusInEvent(QFocusEvent* event)
{
    Q_UNUSED(event);
    _hasFocus = true;
    _cursorVisible = true;
    update();
}

void TextEditMew::focusOutEvent(QFocusEvent* event)
{
    Q_UNUSED(event);
    _hasFocus = false;
    _cursorVisible = false;
    update();
}

void TextEditMew::toggleCursorVisible()
{
    if (_hasFocus) {
        _cursorVisible = !_cursorVisible;
        update();
    }
}

void TextEditMew::updateLayout()
{
    QFont font;
    QFontMetricsF fm(font);

    QStringList lines = _text.split('\n');
    if (lines.isEmpty())
        lines << QString();

    int lineCount = lines.size();
    qreal textHeight = lineCount * fm.height();

    qreal h = textHeight + 2 * _padding;
    if (h < _minHeight)
        h = _minHeight;

    if (!_autoExpand)
        h = _minHeight;

    qreal newHeight = h;
    if (newHeight > _maxSafeHeight)
        newHeight = _maxSafeHeight;

    prepareGeometryChange();
    _bounds = QRectF(0, 0, _maxWidth, newHeight);
}

void TextEditMew::ensureCursorVisible()
{
    // пока ничего не делаем — видимая область = весь виджет
}

void TextEditMew::insertText(const QString& t)
{
    if (_cursorPos < 0 || _cursorPos > _text.size())
        _cursorPos = _text.size();

    _text.insert(_cursorPos, t);
    _cursorPos += t.size();

    updateLayout();
    emit textChanged(_text);
    update();
}

void TextEditMew::deletePreviousChar()
{
    if (_cursorPos <= 0)
        return;

    _text.remove(_cursorPos - 1, 1);
    _cursorPos--;

    updateLayout();
    emit textChanged(_text);
    update();
}

void TextEditMew::deleteNextChar()
{
    if (_cursorPos >= _text.size())
        return;

    _text.remove(_cursorPos, 1);

    updateLayout();
    emit textChanged(_text);
    update();
}

void TextEditMew::moveCursorLeft(bool select)
{
    Q_UNUSED(select);
    if (_cursorPos > 0)
        _cursorPos--;
    ensureCursorVisible();
    update();
}

void TextEditMew::moveCursorRight(bool select)
{
    Q_UNUSED(select);
    if (_cursorPos < _text.size())
        _cursorPos++;
    ensureCursorVisible();
    update();
}

void TextEditMew::moveCursorToEnd()
{
    _cursorPos = _text.size();
    ensureCursorVisible();
    update();
}

void TextEditMew::moveCursorToStart()
{
    _cursorPos = 0;
    ensureCursorVisible();
    update();
}

void TextEditMew::handleReturn()
{
    if (_multiline) {
        insertText("\n");
    } else {
        emit submitted(_text);
    }
}

void TextEditMew::clampHeightIfTooBig()
{
    if (_bounds.height() > _maxSafeHeight) {
        _bounds.setHeight(_maxSafeHeight);
        qDebug() << "[TextEditMew] WARNING: height clamped, text too large";
    }
}

int TextEditMew::positionFromPoint(const QPointF& p) const
{
    QFont font;
    QFontMetricsF fm(font);

    QRectF textRect = _bounds.adjusted(_padding, _padding,
                                       -_padding, -_padding);

    QString text = _text;
    if (text.isEmpty())
        return 0;

    QStringList lines = text.split('\n');
    if (lines.isEmpty())
        lines << QString();

    qreal y = textRect.top();
    qreal lineHeight = fm.height();
    int pos = 0;

    for (int i = 0; i < lines.size(); ++i) {
        const QString& line = lines[i];

        if (p.y() >= y && p.y() <= y + lineHeight) {
            qreal x = textRect.left();
            for (int j = 0; j <= line.size(); ++j) {
                QString sub = line.left(j);
                qreal w = fm.horizontalAdvance(sub);
                if (p.x() < x + w)
                    return pos + j;
            }
            return pos + line.size();
        }

        y += lineHeight;
        pos += line.size() + 1; // + '\n'
    }

    return _text.size();
}

QPointF TextEditMew::cursorPositionToPoint(int pos) const
{
    QFont font;
    QFontMetricsF fm(font);

    QRectF textRect = _bounds.adjusted(_padding, _padding,
                                       -_padding, -_padding);

    QString text = _text;
    if (text.isEmpty())
        return QPointF(textRect.left(), textRect.top() + fm.ascent());

    QStringList lines = text.split('\n');
    if (lines.isEmpty())
        lines << QString();

    int curPos = 0;
    qreal y = textRect.top() + fm.ascent();
    qreal lineHeight = fm.height();

    for (int i = 0; i < lines.size(); ++i) {
        const QString& line = lines[i];
        int lineLen = line.size();

        if (pos <= curPos + lineLen) {
            int offsetInLine = pos - curPos;
            QString sub = line.left(offsetInLine);
            qreal w = fm.horizontalAdvance(sub);
            return QPointF(textRect.left() + w, y);
        }

        curPos += lineLen + 1;
        y += lineHeight;
    }

    return QPointF(textRect.left(), y);
}
