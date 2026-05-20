#include "EditRecipePage.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTextDocument>

EditRecipePage::EditRecipePage(ColorScheme& scheme, QRectF rect)
    : Page(scheme, rect, PageID::editPage),
    _scheme(scheme)
{
}

// ======================================================
//                 SET CONTENT
// ======================================================

void EditRecipePage::setContent(QVector<QString> content)
{
    if (content.size() >= 3) {
        _title    = content[0];
        _category = content[1];
        _steps    = content[2];
    }

    dataAviable = true;
    needRebuild = true;
}

// ======================================================
//                 UPDATE
// ======================================================

void EditRecipePage::update_pages()
{
    if (!needRebuild)
        return;

    needRebuild = false;

    if (_recipeID <= 0)
        return;

    // Загружаем актуальные данные из SQL
    loadRecipeFromSql();

    if (mainRectItem) { delete mainRectItem; mainRectItem = nullptr; }
    if (rightRectItem) { delete rightRectItem; rightRectItem = nullptr; }

    ingredientRows.clear();

    Page::update_pages();
}

// ======================================================
//                 PANELS
// ======================================================

void EditRecipePage::create_main_pannel()
{
    if (!dataAviable)
        return;

    editMode();
}

void EditRecipePage::create_right_pannel()
{
}

void EditRecipePage::editMode()
{
    buildMainPanel();
    buildRightPanel();
}

// ======================================================
//                 MAIN PANEL
// ======================================================

void EditRecipePage::buildMainPanel()
{
    const qreal margin  = 20;
    const qreal spacing = 10;

    QRectF mainRect(0, 0, width / 1.8, height);
    mainRectItem = new QGraphicsRectItem(mainRect, this);
    mainRectItem->setPos(leftRect.width(), 0);
    mainRectItem->setPen(scheme.borderGet());
    mainRectItem->setBrush(scheme.backgroundGet());

    qreal y = margin;

    // TITLE
    qreal titleWidth = mainRectItem->rect().width() - 2 * margin;

    titleEdit = new TextEditMew(_scheme, mainRectItem);
    titleEdit->setPos(margin, y);
    titleEdit->setMaxWidth(titleWidth);
    titleEdit->setMinHeight(50);
    titleEdit->setMaxHeight(50);
    titleEdit->setSingleLine(true);
    titleEdit->setAutoExpand(false);
    titleEdit->setScrollEnabled(false);
    titleEdit->setMaxChars(50);
    titleEdit->setText(_title);

    y += titleEdit->boundingRect().height() + spacing;

    // CATEGORY
    categoryItem = new QGraphicsTextItem(mainRectItem);
    {
        QFont f; f.setPointSize(16);
        categoryItem->setFont(f);
        categoryItem->setDefaultTextColor(_scheme.groupColor());
        categoryItem->setTextWidth(mainRectItem->rect().width() - 2 * margin);
        categoryItem->setPlainText("Категория: " + _category);
        categoryItem->setPos(margin, y);
        y += categoryItem->boundingRect().height() + spacing;
    }

    // SAVE BUTTON
    saveBtn = new ButtonMew(_scheme, mainRectItem);
    saveBtn->setText("Сохранить");
    saveBtn->setPos(margin, y);

    connect(saveBtn, &ButtonMew::clicked, this, [=]() {
        saveRecipe();
        emit goBackToRecipe(_recipeID);
    });

    y += saveBtn->boundingRect().height() + 30;

    // STEPS
    qreal stepsWidth = mainRectItem->rect().width() - 2 * margin;

    stepsEdit = new TextEditMew(_scheme, mainRectItem);
    stepsEdit->setPos(margin, y);
    stepsEdit->setMaxWidth(stepsWidth);
    stepsEdit->setMinHeight(240);
    stepsEdit->setMaxHeight(height * 0.6);
    stepsEdit->setMultiline(true);
    stepsEdit->setAutoExpand(true);
    stepsEdit->setText(_steps);
}

// ======================================================
//                 RIGHT PANEL
// ======================================================

void EditRecipePage::buildRightPanel()
{
    qreal rightWidth = width - (width / 1.8 + leftRect.width());
    QRectF rightRect(0, 0, rightWidth, height);

    rightRectItem = new QGraphicsRectItem(rightRect, this);
    rightRectItem->setPos(width / 1.8 + leftRect.width() + 1, 0);
    rightRectItem->setBrush(_scheme.backgroundGet());
    rightRectItem->setPen(Qt::NoPen);

    auto* header = new QGraphicsTextItem("Ингредиенты", rightRectItem);
    QFont f; f.setPointSize(18); f.setBold(true);
    header->setFont(f);
    header->setDefaultTextColor(_scheme.additionalColorGet());
    header->setPos(20, 20);

    qreal ingTop = header->boundingRect().height() + 40;

    buildIngredientsPanel(rightRectItem, ingTop);

    // === ADD BUTTON ===
    addIngredientBtn = new ButtonMew(_scheme, rightRectItem);
    addIngredientBtn->setText("Добавить");
    addIngredientBtn->setPos(20, ingTop + 10);

    connect(addIngredientBtn, &ButtonMew::clicked, this, [=]() {
        addIngredientRow();
    });

    // === REMOVE BUTTON ===
    removeIngredientBtn = new ButtonMew(_scheme, rightRectItem);
    removeIngredientBtn->setText("Удалить");
    removeIngredientBtn->setPos(addIngredientBtn->boundingRect().width() + 0, ingTop + 10);

    connect(removeIngredientBtn, &ButtonMew::clicked, this, [=]() {
        removeLastIngredientRow();
    });
    repositionIngredientRows();


}

// ======================================================
//                 INGREDIENTS (SQL)
// ======================================================

void EditRecipePage::buildIngredientsPanel(QGraphicsItem* parent, qreal topY)
{
    ingredientRows.clear();

    QVector<Units> units;
    if (db)
        units = db->UnitsTable().Vector();

    QVector<std::tuple<QString,double,int>> list;
    loadIngredientsFromSql(list);

    qreal rowY = topY;
    qreal rowHeight = 45;

    for (auto& t : list)
    {
        QString name  = std::get<0>(t);
        double amount = std::get<1>(t);
        int unitId    = std::get<2>(t);

        IngredientRow* row = new IngredientRow(_scheme, units, parent);
        row->setPos(20, rowY);

        row->setIngredientName(name);
        row->setAmount(QString::number(amount));
        row->setUnit(unitId);

        ingredientRows.append(row);
        rowY += rowHeight;
    }

}

// ======================================================
//                 SAVE
// ======================================================

void EditRecipePage::saveIngredients()
{
    deleteIngredientsForRecipeSql(_recipeID);

    for (auto* row : ingredientRows)
    {
        QString name = row->ingredientName().trimmed();
        if (name.isEmpty())
            continue;

        double amount = row->amount().toDouble();
        int unitId    = row->unitId();

        int ingId = findOrCreateIngredientSql(name);
        if (ingId <= 0)
            continue;

        insertIngredientRowSql(_recipeID, ingId, amount, unitId);
    }
}

void EditRecipePage::saveRecipe()
{
    QString newTitle = titleEdit->text().trimmed();
    QString newSteps = stepsEdit->text();

    if (newTitle.isEmpty()) {
        qDebug() << "EDIT PAGE: empty title";
        return;
    }

    QSqlDatabase dbsql = QSqlDatabase::database("cookbook_connection");
    if (!dbsql.isOpen())
        return;

    QSqlQuery q(dbsql);
    q.prepare("UPDATE recipes SET title = :t, instructions = :s WHERE id = :id");
    q.bindValue(":t", newTitle);
    q.bindValue(":s", newSteps);
    q.bindValue(":id", _recipeID);

    q.exec();

    saveIngredients();
}

// ======================================================
//                 SQL HELPERS
// ======================================================

bool EditRecipePage::loadRecipeFromSql()
{
    QSqlDatabase dbsql = QSqlDatabase::database("cookbook_connection");
    if (!dbsql.isOpen())
        return false;

    QSqlQuery q(dbsql);
    q.prepare(
        "SELECT r.title, r.instructions, c.name "
        "FROM recipes r "
        "JOIN categories c ON r.category_id = c.id "
        "WHERE r.id = :id"
        );
    q.bindValue(":id", _recipeID);

    if (!q.exec() || !q.next())
        return false;

    _title    = q.value(0).toString();
    _steps    = q.value(1).toString();
    _category = q.value(2).toString();

    return true;
}

void EditRecipePage::loadIngredientsFromSql(QVector<std::tuple<QString,double,int>>& out)
{
    out.clear();

    QSqlDatabase dbsql = QSqlDatabase::database("cookbook_connection");
    if (!dbsql.isOpen())
        return;

    QSqlQuery q(dbsql);
    q.prepare(
        "SELECT i.name, ri.amount, ri.unit_id "
        "FROM recipe_ingredients ri "
        "JOIN ingredients i ON i.id = ri.ingredient_id "
        "WHERE ri.recipe_id = :id"
        );
    q.bindValue(":id", _recipeID);

    if (!q.exec())
        return;

    while (q.next()) {
        out.append(std::make_tuple(
            q.value(0).toString(),
            q.value(1).toDouble(),
            q.value(2).toInt()
            ));
    }
}

int EditRecipePage::findOrCreateIngredientSql(const QString& name)
{
    QSqlDatabase dbsql = QSqlDatabase::database("cookbook_connection");
    if (!dbsql.isOpen())
        return 0;

    // find
    {
        QSqlQuery q(dbsql);
        q.prepare("SELECT id FROM ingredients WHERE LOWER(name)=LOWER(:n) LIMIT 1");
        q.bindValue(":n", name);

        if (q.exec() && q.next())
            return q.value(0).toInt();
    }

    // create
    {
        QSqlQuery q(dbsql);
        q.prepare("INSERT INTO ingredients (name) VALUES (:n)");
        q.bindValue(":n", name);

        if (!q.exec())
            return 0;

        return q.lastInsertId().toInt();
    }
}

void EditRecipePage::deleteIngredientsForRecipeSql(int recipeId)
{
    QSqlDatabase dbsql = QSqlDatabase::database("cookbook_connection");
    if (!dbsql.isOpen())
        return;

    QSqlQuery q(dbsql);
    q.prepare("DELETE FROM recipe_ingredients WHERE recipe_id = :id");
    q.bindValue(":id", recipeId);
    q.exec();
}

void EditRecipePage::insertIngredientRowSql(int recipeId, int ingredientId, double amount, int unitId)
{
    QSqlDatabase dbsql = QSqlDatabase::database("cookbook_connection");
    if (!dbsql.isOpen())
        return;

    QSqlQuery q(dbsql);
    q.prepare(
        "INSERT INTO recipe_ingredients (recipe_id, ingredient_id, amount, unit_id) "
        "VALUES (:r, :i, :a, :u)"
        );
    q.bindValue(":r", recipeId);
    q.bindValue(":i", ingredientId);
    q.bindValue(":a", amount);
    q.bindValue(":u", unitId);

    q.exec();
}

void EditRecipePage::addIngredientRow()
{
    if (ingredientRows.size() >= 14)
        return; // или показать сообщение

    QVector<Units> units;
    if (db)
        units = db->UnitsTable().Vector();

    IngredientRow* row = new IngredientRow(_scheme, units, rightRectItem);
    ingredientRows.append(row);

    repositionIngredientRows();
}


void EditRecipePage::removeLastIngredientRow()
{
    if (ingredientRows.size() <= 1)
        return;

    IngredientRow* row = ingredientRows.last();
    ingredientRows.removeLast();
    delete row;

    repositionIngredientRows();
}
void EditRecipePage::repositionIngredientRows()
{
    qreal startX = 10;
    qreal rowHeight = 45;

    // Верхняя граница блока ингредиентов
    qreal y = 20 + 20 + 40; // header height + margins

    for (auto* row : ingredientRows)
    {
        row->setPos(startX, y);
        y += rowHeight;
    }

    if (addIngredientBtn)
        addIngredientBtn->setPos(startX, y + 10);

    if (removeIngredientBtn)
        removeIngredientBtn->setPos(startX + addIngredientBtn->boundingRect().width() + 0, y + 10);
}


