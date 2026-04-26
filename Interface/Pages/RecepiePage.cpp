#include "RecepiePage.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QPen>
#include <QDebug>

RecepiePage::RecepiePage(ColorScheme &scheme, QRectF rect)
    : Page(scheme, rect, PageID::recepie),
    _scheme(scheme)
{
}

void RecepiePage::create_main_pannel()
{
    // центральная панель
    QRectF mainRect = QRectF(0, 0, width / 1.8, height);
    mainRectItem = new QGraphicsRectItem(mainRect);
    mainRectItem->setParentItem(this);
    mainRectItem->setPos(leftRect.width(), 0);
    mainRectItem->setPen(QPen(scheme.borderGet(), 1));

    // если ID не установлен — просто пишем заглушку
    if (_recipeID < 0)
    {
        QGraphicsTextItem* noID = new QGraphicsTextItem("No recipe selected", mainRectItem);
        noID->setDefaultTextColor(Qt::red);
        noID->setFont(QFont("Arial", 20));
        noID->setPos(20, 20);
        return;
    }

    // подключаемся к базе
    QSqlDatabase db = QSqlDatabase::database("cookbook_connection");
    if (!db.isOpen())
    {
        QGraphicsTextItem* err = new QGraphicsTextItem("DB not open!", mainRectItem);
        err->setDefaultTextColor(Qt::red);
        err->setFont(QFont("Arial", 20));
        err->setPos(20, 20);
        return;
    }

    // SQL-запрос для одного рецепта
    QString sql =
        "SELECT r.title, r.instructions, c.name AS category, "
        "GROUP_CONCAT(i.name || ' — ' || ri.amount || ' ' || u.short_name, '\n') AS ingredients "
        "FROM recipes r "
        "JOIN categories c ON r.category_id = c.id "
        "LEFT JOIN recipe_ingredients ri ON ri.recipe_id = r.id "
        "LEFT JOIN ingredients i ON i.id = ri.ingredient_id "
        "LEFT JOIN units u ON u.id = ri.unit_id "
        "WHERE r.id = :id "
        "GROUP BY r.id;";

    QSqlQuery q(db);
    q.prepare(sql);
    q.bindValue(":id", _recipeID);

    if (!q.exec() || !q.next())
    {
        QGraphicsTextItem* err = new QGraphicsTextItem("Recipe not found", mainRectItem);
        err->setDefaultTextColor(Qt::red);
        err->setFont(QFont("Arial", 20));
        err->setPos(20, 20);
        return;
    }

    QString title       = q.value("title").toString();
    QString category    = q.value("category").toString();
    QString ingredients = q.value("ingredients").toString();
    QString steps       = q.value("instructions").toString();

    // ===== TITLE =====
    titleItem = new QGraphicsTextItem(mainRectItem);
    titleItem->setDefaultTextColor(_scheme.titleColor());
    titleItem->setFont(QFont("Arial", 24, QFont::Bold));
    titleItem->setPlainText(title);
    titleItem->setPos(20, 20);

    // ===== CATEGORY =====
    categoryItem = new QGraphicsTextItem(mainRectItem);
    categoryItem->setDefaultTextColor(_scheme.groupColor());
    categoryItem->setFont(QFont("Arial", 16));
    categoryItem->setPlainText("Category: " + category);
    categoryItem->setPos(20, 70);

    // ===== INGREDIENTS =====
    ingredientsItem = new QGraphicsTextItem(mainRectItem);
    ingredientsItem->setDefaultTextColor(_scheme.additionalColorGet());
    ingredientsItem->setFont(QFont("Arial", 14));
    ingredientsItem->setTextWidth(mainRect.width() - 40);
    ingredientsItem->setPlainText("Ingredients:\n" + ingredients);
    ingredientsItem->setPos(20, 120);

    // ===== STEPS =====
    stepsItem = new QGraphicsTextItem(mainRectItem);
    stepsItem->setDefaultTextColor(_scheme.textColorGet());
    stepsItem->setFont(QFont("Arial", 14));
    stepsItem->setTextWidth(mainRect.width() - 40);
    stepsItem->setPlainText("Steps:\n" + steps);
    stepsItem->setPos(20, ingredientsItem->boundingRect().bottom() + 20);
}
