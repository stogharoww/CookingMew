#pragma once
#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QPainter>
#include <QPen>
#include <QGraphicsSceneWheelEvent>
#include <QRectF>
#include <QDebug>

class AutoGrowBox : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit AutoGrowBox(qreal width,
                         qreal minHeight,
                         qreal maxHeight,
                         QGraphicsItem* parent = nullptr)
        : QGraphicsObject(parent),
        _width(width),
        _minHeight(minHeight),
        _maxHeight(maxHeight),
        _currentHeight(minHeight)
    {
        setFlag(ItemClipsChildrenToShape, true);
        setAcceptedMouseButtons(Qt::AllButtons);
    }

    QRectF boundingRect() const override
    {
        return QRectF(0, 0, _width, _currentHeight);
    }

    void paint(QPainter* p,
               const QStyleOptionGraphicsItem*,
               QWidget*) override
    {
        p->setRenderHint(QPainter::Antialiasing, true);

        // фон
        //p->setBrush(Qt::NoBrush);
        p->setBrush(QColor("#1e1e1e"));

        // рамка — только если включена
        if (_drawBorder)
            p->setPen(QPen(_borderColor, 2));
        else
            p->setPen(Qt::NoPen);

        p->drawRoundedRect(boundingRect(), 10, 10);
    }

    void setContentHeight(qreal h)
    {
        qreal minH = qMin(_minHeight, _maxHeight);
        qreal maxH = qMax(_minHeight, _maxHeight);

        qreal newH = qBound(minH, h, maxH);

        if (newH != _currentHeight)
        {
            prepareGeometryChange();
            _currentHeight = newH;
            update();
        }
    }

    // === Новые методы ===
    void setBorderVisible(bool on)
    {
        _drawBorder = on;
        update();
    }

    void setBorderColor(const QColor& c)
    {
        _borderColor = c;
        update();
    }

private:
    qreal _width;
    qreal _minHeight;
    qreal _maxHeight;
    qreal _currentHeight;

    bool _drawBorder = false;
    QColor _borderColor = QColor("#888");
};






class MewScrollArea : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit MewScrollArea(QRectF viewportRect, QGraphicsItem* parent = nullptr);

    void setContent(QGraphicsObject* item);
    void setContentItem(QGraphicsItem* item);


    // ключевые методы
    void updateContentGeometry();
    void scrollToBottom();
    void scrollToTop();

    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

    void ensureVisibleY(qreal y);

    void setViewportHeight(qreal h);

    QGraphicsObject* _content = nullptr;
signals:
    void openRecipe(int recipeID);

protected:
    void wheelEvent(QGraphicsSceneWheelEvent* event) override;

private:
    QRectF _viewport;


    qreal _scrollY = 0;
    qreal _contentHeight = 0;
    qreal _contentWidth  = 0;
};
