#pragma once

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QBrush>
#include <QGraphicsTextItem>
#include <QFontDatabase>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QVector>
#include "../ColorScheme.h"


class Button: public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Button(ColorScheme& colorScheme, QGraphicsItem* parent = nullptr)
        : QObject(), QGraphicsItem(parent), scheme(colorScheme)
    {
        setAcceptHoverEvents(true);
        setColorScheme();

    }
    virtual ~Button() = default;

    ///
    /// \brief set_pixmap: Рисует картинку внутри кнопки
    /// \param path: путь до картинки
    ///
    virtual void set_pixmap(QString& path) = 0;


    virtual QRectF boundingRect() const override;
    ///
    /// \brief change_main_color: меняет основной и дополнительный цвет
    ///
    virtual void change_main_color();

    virtual void updateScheme(ColorScheme& colorScheme);


signals:
    void clicked();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override;

    ///
    /// \brief change_color: меняет цвет кнопки после нажатия
    ///
    //virtual void change_color();
    ///
    /// \brief set_form: устанавливает форму кнопки
    ///
    virtual void set_form() = 0;

    virtual void setBrush(QBrush &brush);
    virtual void setSecondBrush(QBrush &brush);

    virtual void setBoundingRect(QRectF &rect);

    virtual void setColorScheme();


    bool _changed = false;
    ColorScheme scheme;
    QBrush _brush;
    QBrush _second_brush;
    qreal _width;
    qreal _height;
    QRectF _b_rect;
    QColor baseColor = QColor("#730000");      // исходный цвет
    QColor secondColor = QColor("#0d0000");
    QColor hoverColor;     // цвет при наведении
    QColor pressedColor;   // цвет при нажатии
    QColor currentColor;   // текущий цвет кнопки


};


