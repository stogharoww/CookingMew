#pragma once

#include "../Virtual/Button.h"
#include <QGraphicsPixmapItem>
#include <QColor>
#include <QSize>
#include <QGraphicsTextItem>
#include <QString>

class ButtonMew : public Button
{
public:
    ButtonMew(ColorScheme& scheme, QGraphicsItem* paren = nullptr);

    ///
    /// \brief set_pixmap: Рисует картинку внутри кнопки
    /// \param path: путь до картинки
    ///
    void set_pixmap(QString& path) override;
    ///
    /// \brief set_text: Устанавливает текст кнопки
    /// \param text: текст
    ///
    void set_text(QString& text);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;







private:
    void set_form() override;

    QColor _textColor;
    QGraphicsPixmapItem *_pixmap;
    QSize _buttonSize;
    QString _text = "";
    QRect rect;
    QPoint firstEllipse;
    QPoint secondEllipse;
    QPoint rectPos;
    qreal textSize = 1;




};
