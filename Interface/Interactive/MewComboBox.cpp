#include "MewComboBox.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

MewComboBox::MewComboBox(ColorScheme& scheme_, QGraphicsItem* parent)
    : QGraphicsObject(parent),
    scheme(scheme_)
{
}

void MewComboBox::setSize(qreal w, qreal h)
{
    width = w;
    height = h;
    prepareGeometryChange();
}

QRectF MewComboBox::boundingRect() const
{
    return QRectF(0, 0, width, height);
}

void MewComboBox::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*)
{
    p->setRenderHint(QPainter::Antialiasing);

    p->setBrush(scheme.backgroundGet());
    p->setPen(scheme.borderGet());
    p->drawRoundedRect(boundingRect(), 6, 6);

    p->setPen(scheme.textColorGet());
    if (!items.isEmpty())
        p->drawText(QRectF(5, 0, width - 10, height),
                    Qt::AlignVCenter | Qt::AlignLeft,
                    items[current].text);
}

void MewComboBox::addItem(const QString& text, int data)
{
    items.append({text, data});
}

QString MewComboBox::itemText(int index) const
{
    return items[index].text;
}

int MewComboBox::itemData(int index) const
{
    return items[index].data;
}

void MewComboBox::setCurrentIndex(int index)
{
    if (index >= 0 && index < items.size()) {
        current = index;
        update();
        emit changed(index);
    }
}

QString MewComboBox::currentText() const
{
    return items.isEmpty() ? QString() : items[current].text;
}

int MewComboBox::currentData() const
{
    return items.isEmpty() ? -1 : items[current].data;
}

void MewComboBox::mousePressEvent(QGraphicsSceneMouseEvent*)
{
    if (items.isEmpty()) return;

    current = (current + 1) % items.size();
    update();
    emit changed(current);
}
