#pragma once
#include <QGraphicsObject>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QFontMetricsF>
#include <QGraphicsSceneWheelEvent>
#include "../ColorScheme.h"

class TextEditMew : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit TextEditMew(ColorScheme& scheme,
                         QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override;

    // Текст
    void setText(const QString& text);
    QString text() const;

    // Плейсхолдер
    void setPlaceholder(const QString& text);

    // Режимы
    void setMultiline(bool enabled);      // true = многострочный
    void setSingleLine(bool enabled);     // удобный алиас
    void setAutoExpand(bool enabled);     // если false — фиксированная высота
    void setMaxChars(int n);              // -1 = без лимита

    // Геометрия
    void setMinHeight(qreal h);
    void setMaxHeight(qreal h);
    void setMaxWidth(qreal w);

    // Высота текста (полная, без учёта maxHeight)
    qreal textHeight() const;

    void setScrollEnabled(bool enabled) { _scrollEnabled = enabled; }



signals:
    void textChanged(const QString& text);
    void submitted(const QString& text);
    void heightChanged(qreal newHeight);
    void cursorMoved(qreal cursorY);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void wheelEvent(QGraphicsSceneWheelEvent* event) override;

private slots:
    void toggleCursorVisible();

private:
    bool _scrollEnabled = true;
    void updateLayout();
    void insertText(const QString& t);
    void deletePreviousChar();
    void deleteNextChar();
    void moveCursorLeft(bool select);
    void moveCursorRight(bool select);
    void moveCursorToEnd();
    void moveCursorToStart();
    void ensureCursorVisible();

    bool _suppressEnsureVisible = true;


    int     positionFromPoint(const QPointF& p) const;
    QPointF cursorPositionToPoint(int pos) const;

    // состояние
    QString _text;
    QString _placeholder;

    bool _multiline   = true;
    bool _autoExpand  = true;

    int  _cursorPos      = 0;
    bool _cursorVisible  = true;
    QTimer* _cursorTimer = nullptr;

    // геометрия
    QRectF _bounds;
    qreal  _padding      = 6.0;
    qreal  _minHeight    = 40.0;
    qreal  _maxHeight    = 200.0;
    qreal  _maxWidth     = 300.0;
    qreal  _currentHeight = 40.0;

    // скролл
    qreal _scrollOffset = 0.0;

    // оформление
    qreal _borderWidth = 2.0;
    ColorScheme& _scheme;
    QColor _borderColor;
    QColor _focusedBorderColor;
    QColor _backgroundColor;
    QColor _textColor;
    QColor _placeholderColor;

    bool _hasFocus = false;

    int _maxChars = -1; // -1 = без лимита
};
