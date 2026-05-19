#pragma once

#include <QGraphicsObject>
#include "../Interactive/TextEditMew.h"
#include "../Interactive/MewComboBox.h"
#include "../ColorScheme.h"
#include "../../Database/database.h"   // здесь лежит Units

class IngredientRow : public QGraphicsObject
{
    Q_OBJECT

public:
    IngredientRow(ColorScheme& scheme,
                  const QVector<Units>& units,
                  QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

    void setIngredientName(const QString& name);
    void setAmount(const QString& amount);
    void setUnit(int unitId);

    QString ingredientName() const;
    QString amount() const;
    int unitId() const;

private:
    ColorScheme _scheme;

    TextEditMew* nameEdit = nullptr;
    TextEditMew* amountEdit = nullptr;
    MewComboBox* unitCombo = nullptr;

    QVector<Units> _units;

    qreal _width = 0;
    qreal _height = 40;
};
