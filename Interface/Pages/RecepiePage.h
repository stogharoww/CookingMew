#pragma once

#include "../Virtual/Page.h"
#include "../ColorScheme.h"
#include "../Interactive/MewScrollArea.h"
#include "../Interactive/TextEditMew.h"
#include "../Interactive/ButtonMew.h"
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include "../../Database/database.h"

class RecepiePage : public Page
{
    Q_OBJECT
public:
    RecepiePage(ColorScheme& scheme, QRectF rect);

    void setRecipeID(int id)        { _recipeID = id; }
    void setDatabase(DataBase* db_) { db = db_; }

private:
    void create_main_pannel() override;
    void create_right_pannel() override;

    class ContentRoot : public QGraphicsObject
    {
    public:
        explicit ContentRoot(QGraphicsItem* parent = nullptr)
            : QGraphicsObject(parent) {}

        QRectF boundingRect() const override { return _bounds; }
        void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override {}

        void setBounds(const QRectF& r)
        {
            prepareGeometryChange();
            _bounds = r;
        }

    private:
        QRectF _bounds;
    };

    void buildContent(ContentRoot* root,
                      const QString& title,
                      const QString& category,
                      const QString& ingredients,
                      const QString& steps);

private:
    DataBase* db = nullptr;
    ColorScheme _scheme;
    int _recipeID = -1;

    bool _editMode = false;

    QGraphicsRectItem* mainRectItem = nullptr;
    QGraphicsRectItem* rightRectItem = nullptr;

    MewScrollArea* scrollArea = nullptr;

    QString _ingredientsRaw;

    // для режима редактирования справа
    TextEditMew* ingredientsEdit = nullptr;
};
