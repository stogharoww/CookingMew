#include "MewItem.h"
#include <QFont>



MewItem::MewItem(ColorScheme &scheme, QRectF &globalRect, QGraphicsItem *paren)
    : Button(scheme, paren),
    _scheme(scheme)
{
    //НОВОЕ!!
    titleItem = new QGraphicsTextItem(this);
    titleItem->setDefaultTextColor(Qt::white);
    titleItem->setPos(10, 10);
    //
    mainRect = QRectF(4, 4, globalRect.width() - 8, 500);
    mainRectItem = new QGraphicsRectItem(mainRect);
    mainRectItem->setParentItem(this);
    mainRectItem->setPen(QPen(scheme.borderGet(), 1));
    //mainRectItem->setBrush(QBrush(scheme.borderGet()));

    //colors
    //colors.resize(5);
    // title, group, recepie, ingredients, tag - цвета в этом порядке
    colors = {
        _scheme.titleColor(), _scheme.groupColor(),
        _scheme.textColorGet(), _scheme.additionalColorGet(),
        _scheme.tagColor()
    };
    QString title("title"), group("group"), tag("tag"), recepie("recepie..."), ingredients("ingredients");
    set_content(title, group, recepie, ingredients, tag);
}

void MewItem::setTitle(const QString& t)
{
    m_title = t;
    content[0] = t;
    //update();
    titleItem->setPlainText(t);
}

void MewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen pen;
    if (content.size() == 0)
        return;
    int count = 0;
    QVector<int> contentSize = {
        18, 20, 14, 14, 14
    };
    for (const auto& col : colors){
        pen.setColor(col);
        painter->setPen(pen);
        QPointF textPos(mainRect.x() + 30, 20 + 20 * count);
        font = QFont(arial, contentSize[count]);
        painter->setFont(font);
        painter->drawText(textPos, content[count]);


        count++;
    }
}

QRectF MewItem::boundingRect() const
{
    return mainRect;
}

void MewItem::set_content(QString &title, QString &group, QString &recepie, QString &ingredients, QString &tag)
{
    content = {
        title, group, recepie, ingredients, tag
    };
}
