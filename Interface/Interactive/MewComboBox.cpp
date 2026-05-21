#include "MewComboBox.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QEvent>

MewComboBox* MewComboBox::openedCombo = nullptr;

// ===============================
//        PopupRect class
// ===============================

class PopupRect : public QGraphicsRectItem
{
public:
    MewComboBox* owner;

    PopupRect(MewComboBox* o)
        : QGraphicsRectItem(o), owner(o)
    {
        setAcceptHoverEvents(true);
        setAcceptedMouseButtons(Qt::LeftButton);
    }

    void paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) override
    {
        p->setRenderHint(QPainter::Antialiasing);

        // фон
        p->setBrush(owner->scheme.backgroundGet());
        p->setPen(owner->scheme.borderGet());
        p->drawRect(rect());

        qreal itemH = owner->height;

        // подсветка
        if (owner->hoveredIndex >= 0)
        {
            qreal y = rect().y() + owner->hoveredIndex * itemH;

            p->setBrush(QColor(255, 255, 255, 40));
            p->setPen(Qt::NoPen);
            p->drawRect(rect().x(), y, owner->width, itemH);
        }

        // === ШРИФТ ===
        p->setFont(owner->font);
        p->setPen(owner->scheme.textColorGet());

        // текст пунктов
        for (int i = 0; i < owner->items.size(); ++i)
        {
            qreal y = rect().y() + i * itemH;
            p->drawText(QRectF(rect().x() + 5, y, owner->width - 10, itemH),
                        Qt::AlignVCenter | Qt::AlignLeft,
                        owner->items[i].text);
        }
    }

    // hover
    void hoverMoveEvent(QGraphicsSceneHoverEvent* e) override
    {
        qreal itemH = owner->height;
        int index = int((e->pos().y() - rect().y()) / itemH);

        if (index >= 0 && index < owner->items.size())
            owner->hoveredIndex = index;
        else
            owner->hoveredIndex = -1;

        update();
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent*) override
    {
        owner->hoveredIndex = -1;
        update();
    }

    // click
    void mousePressEvent(QGraphicsSceneMouseEvent* e) override
    {
        qreal itemH = owner->height;
        int index = int((e->pos().y() - rect().y()) / itemH);

        if (index >= 0 && index < owner->items.size())
        {
            owner->setCurrentIndex(index);
            owner->hidePopup();
        }
    }
};

// ===============================
//        MewComboBox
// ===============================

MewComboBox::MewComboBox(ColorScheme& scheme_, QGraphicsItem* parent)
    : QGraphicsObject(parent),
    scheme(scheme_)
{
    // === КАСТОМНЫЙ ШРИФТ ===
    int id = QFontDatabase::addApplicationFont(":/fonts/resourses/fonts/ARIAL.TTF");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    font = QFont(family, 12); // можно менять размер
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

    // === ШРИФТ ===
    p->setFont(font);
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
    if (popupVisible)
        hidePopup();
    else
        showPopup();
}

// ===============================
//       POPUP LOGIC
// ===============================

void MewComboBox::raiseZ()
{
    if (parentItem())
        parentItem()->setZValue(2000);

    this->setZValue(2001);

    if (popupRect)
        popupRect->setZValue(2002);
}

void MewComboBox::restoreZ()
{
    if (parentItem())
        parentItem()->setZValue(0);

    this->setZValue(0);
}

void MewComboBox::showPopup()
{
    if (items.isEmpty())
        return;

    if (openedCombo && openedCombo != this)
        openedCombo->hidePopup();

    openedCombo = this;

    if (!popupRect)
        popupRect = new PopupRect(this);

    rebuildPopup();
    popupVisible = true;

    popupRect->setVisible(true);

    raiseZ();
}

void MewComboBox::hidePopup()
{
    popupVisible = false;

    if (popupRect)
        popupRect->setVisible(false);

    if (openedCombo == this)
        openedCombo = nullptr;

    hoveredIndex = -1;
    restoreZ();
}

void MewComboBox::rebuildPopup()
{
    qreal itemH = height;
    qreal popupH = items.size() * itemH;

    QPointF pScene = mapToScene(0, 0);
    qreal sceneBottom = scene()->sceneRect().bottom();

    bool openDown = !(pScene.y() + height + popupH > sceneBottom);

    if (openDown)
        popupRect->setRect(0, height + 2, width, popupH);
    else
        popupRect->setRect(0, -popupH - 2, width, popupH);
}

bool MewComboBox::eventFilter(QObject*, QEvent*)
{
    return false;
}
