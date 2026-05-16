#pragma once

#include <QGraphicsObject>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QFontMetricsF>
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
    void setMultiline(bool enabled);
    void setAutoExpand(bool enabled);

    // Геометрия
    void setMinHeight(qreal h);
    void setMaxWidth(qreal w);

signals:
    void textChanged(const QString& text);
    void submitted(const QString& text);

protected:
    // События
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;

private slots:
    void toggleCursorVisible();

private:
    // Внутренние методы
    void updateLayout();
    void ensureCursorVisible();
    void insertText(const QString& t);
    void deletePreviousChar();
    void deleteNextChar();
    void moveCursorLeft(bool select);
    void moveCursorRight(bool select);
    void moveCursorToEnd();
    void moveCursorToStart();
    void handleReturn();
    void clampHeightIfTooBig();

    int     positionFromPoint(const QPointF& p) const;
    QPointF cursorPositionToPoint(int pos) const;

    // Состояние текста
    QString _text;
    QString _placeholder;
    bool    _multiline   = true;
    bool    _autoExpand  = true;

    // Курсор и выделение
    int  _cursorPos      = 0;
    int  _selectionStart = -1;
    int  _selectionEnd   = -1;
    bool _cursorVisible  = true;
    QTimer* _cursorTimer = nullptr;

    // Геометрия
    QRectF _bounds;
    qreal  _padding      = 10.0;
    qreal  _minHeight    = 40.0;
    qreal  _maxWidth     = 300.0;

    // Ограничение по высоте
    qreal  _maxSafeHeight = 4000.0;

    // Стиль
    ColorScheme& _scheme;
    QColor _borderColor;
    QColor _focusedBorderColor;
    QColor _backgroundColor;
    QColor _textColor;
    QColor _placeholderColor;

    bool _hasFocus = false;
};
