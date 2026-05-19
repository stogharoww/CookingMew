#include "EditRecipePage.h"
#include <QTextDocument>
#include <QSqlQuery>
#include <QSqlError>

EditRecipePage::EditRecipePage(ColorScheme& scheme, QRectF rect)
    : Page(scheme, rect, PageID::editPage),
    _scheme(scheme)
{}

void EditRecipePage::setContent(QVector<QString> content)
{
    _title = content[0];
    _category = content[1];
    _steps = content[2];
    _ingredients = content[3];
    dataAviable = true;
}

void EditRecipePage::update_pages()
{
    Page::update_pages();
}

void EditRecipePage::create_main_pannel()
{
    if (!dataAviable) {
        qDebug() << "EDIT PAGE:: Data is not available";
        return;
    }

    editMode();
}

void EditRecipePage::create_right_pannel()
{
    // unused
}

void EditRecipePage::editMode()
{
    buildMainPanel();
    buildRightPanel();
}

//
// ===================== MAIN PANEL =====================
//

void EditRecipePage::buildMainPanel()
{
    const qreal margin = 20;
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
    qreal stepsMinH = 240;
    qreal stepsMaxH = height * 0.6;

    stepsEdit = new TextEditMew(_scheme, mainRectItem);
    stepsEdit->setPos(margin, y);
    stepsEdit->setMaxWidth(stepsWidth);
    stepsEdit->setMinHeight(stepsMinH);
    stepsEdit->setMaxHeight(stepsMaxH);
    stepsEdit->setMultiline(true);
    stepsEdit->setAutoExpand(true);
    stepsEdit->setText(_steps);
}

//
// ===================== RIGHT PANEL =====================
//

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
}

//
// ===================== INGREDIENTS PANEL =====================
//

QVector<Recipeingred> EditRecipePage::ingredientsForRecipe(int recipeId)
{
    QVector<Recipeingred> result;

    for (const auto& ri : db->RecipeIngredientsTable().Vector())
        if (ri.recipe_id == recipeId)
            result.append(ri);

    return result;
}

ingredient EditRecipePage::getIngredientById(int id)
{
    auto& h = db->IngredientsTable().Hash();
    return h.contains(id) ? h[id] : ingredient{};
}

int EditRecipePage::findOrCreateIngredient(const QString& name)
{
    auto& vec = db->IngredientsTable().Vector();

    for (const auto& ing : vec)
        if (ing.name.compare(name, Qt::CaseInsensitive) == 0)
            return ing.id;

    ingredient ing;
    ing.name = name;

    db->IngredientsTable().Add(ing);
    db->IngredientsTable().Read();

    for (const auto& ing2 : db->IngredientsTable().Vector())
        if (ing2.name == name)
            return ing2.id;

    return 0;
}

void EditRecipePage::deleteIngredientsForRecipe(int recipeId)
{
    for (const auto& ri : db->RecipeIngredientsTable().Vector())
        if (ri.recipe_id == recipeId)
            db->RecipeIngredientsTable().Delete(ri);

    db->RecipeIngredientsTable().Read();
}

void EditRecipePage::insertIngredientRow(const Recipeingred& ri)
{
    db->RecipeIngredientsTable().Add(ri);
}

void EditRecipePage::buildIngredientsPanel(QGraphicsItem* parent, qreal topY)
{
    ingredientRows.clear();

    QVector<Units> units = db->UnitsTable().Vector();
    QVector<Recipeingred> list = ingredientsForRecipe(_recipeID);

    qreal rowY = topY;
    qreal rowHeight = 45;

    for (const auto& ri : list)
    {
        IngredientRow* row = new IngredientRow(_scheme, units, parent);
        row->setPos(20, rowY);

        ingredient ing = getIngredientById(ri.ingredient_id);
        row->setIngredientName(ing.name);
        row->setAmount(QString::number(ri.amount));
        row->setUnit(ri.unit_id);

        ingredientRows.append(row);
        rowY += rowHeight;
    }
}

//
// ===================== SAVE =====================
//

void EditRecipePage::saveIngredients()
{
    deleteIngredientsForRecipe(_recipeID);

    for (auto* row : ingredientRows)
    {
        QString name = row->ingredientName();
        if (name.trimmed().isEmpty())
            continue;

        double amount = row->amount().toDouble();
        int unitId = row->unitId();

        int ingId = findOrCreateIngredient(name);

        Recipeingred ri;
        ri.recipe_id = _recipeID;
        ri.ingredient_id = ingId;
        ri.amount = amount;
        ri.unit_id = unitId;

        insertIngredientRow(ri);
    }
}

void EditRecipePage::saveRecipe()
{
    if (!db) return;

    QString newTitle = titleEdit->text();
    QString newSteps = stepsEdit->text();

    QSqlDatabase dbsql = QSqlDatabase::database("cookbook_connection");
    if (!dbsql.isOpen()) return;

    QSqlQuery q(dbsql);
    q.prepare("UPDATE recipes SET title = :title, instructions = :steps WHERE id = :id");
    q.bindValue(":title", newTitle);
    q.bindValue(":steps", newSteps);
    q.bindValue(":id", _recipeID);

    if (!q.exec())
        qDebug() << "UPDATE FAILED:" << q.lastError();
    else
        qDebug() << "Recipe updated OK";

    saveIngredients();
}
