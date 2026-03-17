#pragma once

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QBrush>
#include <QGraphicsTextItem>
#include <QFontDatabase>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>

class Button: public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit Button(QGraphicsItem* parent = nullptr)
        : QObject(), QGraphicsRectItem(parent)
    {
        setAcceptHoverEvents(true);
    }
    virtual ~Button() = default;

    ///
    /// \brief set_pixmap: Рисует картинку внутри кнопки
    /// \param path: путь до картинки
    ///
    virtual void set_pixmap(QString& path) = 0;
    ///
    /// \brief set_color: устанавливает цвет кнопки
    /// \param col: Цвет в формате RGB
    ///
    virtual void set_color(QColor& col) = 0;


signals:
    void clicked();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event) override = 0;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override = 0;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override = 0;

    ///
    /// \brief change_color: меняет цвет кнопки после нажатия
    ///
    virtual void change_color() = 0;
    ///
    /// \brief set_form: устанавливает форму кнопки
    ///
    virtual void set_form() = 0;


    bool _changed = false;
};

