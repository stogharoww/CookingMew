#include "IngredientRow.h"
#include <QPainter>

IngredientRow::IngredientRow(ColorScheme& scheme,
                             const QVector<Units>& units,
                             QGraphicsItem* parent)
    : QGraphicsObject(parent),
    _scheme(scheme),
    _units(units)
{
    _width = parent ? parent->boundingRect().width() : 400;

    qreal col1 = _width * 0.50;   // название
    qreal col2 = _width * 0.20;   // количество
    qreal col3 = _width * 0.30;   // единица

    // === NAME ===
    nameEdit = new TextEditMew(_scheme, this);
    nameEdit->setPos(0, 0);
    nameEdit->setMinHeight(_height);
    nameEdit->setMaxHeight(_height);
    nameEdit->setSingleLine(true);
    nameEdit->setAutoExpand(false);   // вызывает updateLayout()
    nameEdit->setMaxWidth(col1 - 10); // вызывает updateLayout() ЕЩЁ РАЗ


    // === AMOUNT ===
    amountEdit = new TextEditMew(_scheme, this);
    amountEdit->setPos(col1, 0);
    amountEdit->setMaxWidth(col2 - 10);
    amountEdit->setMinHeight(_height);
    amountEdit->setMaxHeight(_height);
    amountEdit->setSingleLine(true);
    amountEdit->setAutoExpand(false);

    // === UNIT COMBO ===
    unitCombo = new MewComboBox(_scheme, this);
    unitCombo->setPos(col1 + col2, 0);
    unitCombo->setSize(col3 - 10, _height);

    for (const Units& u : _units)
        unitCombo->addItem(u.short_name, u.id);

    nameEdit->setZValue(10);
    amountEdit->setZValue(10);
    unitCombo->setZValue(10);
    this->setZValue(0);

}

QRectF IngredientRow::boundingRect() const
{
    return QRectF(0, 0, _width, _height);
}

void IngredientRow::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*)
{
    p->setBrush(QColor(255, 0, 0, 80));
    p->drawRect(boundingRect());

}


// ================== SETTERS ==================

void IngredientRow::setIngredientName(const QString& name)
{
    nameEdit->setText(name);
}

void IngredientRow::setAmount(const QString& amount)
{
    amountEdit->setText(amount);
}

void IngredientRow::setUnit(int unitId)
{
    for (int i = 0; i < unitCombo->count(); ++i) {
        if (unitCombo->itemData(i) == unitId) {
            unitCombo->setCurrentIndex(i);
            return;
        }
    }
}

// ================== GETTERS ==================

QString IngredientRow::ingredientName() const
{
    return nameEdit->text();
}

QString IngredientRow::amount() const
{
    return amountEdit->text();
}

int IngredientRow::unitId() const
{
    return unitCombo->currentData();
}
