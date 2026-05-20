#include "PostPage.h"
#include <QSqlQuery>
#include <QSqlError>

PostPage::PostPage(ColorScheme& scheme, QRectF rect)
    : Page(scheme, rect, PageID::post),
    _scheme(scheme)
{
}

// ======================================================
//                 UPDATE
// ======================================================

void PostPage::update_pages()
{
    if (!needRebuild)
        return;

    if (!db)
    {
        qDebug() << "POST PAGE: db is null";
        return;
    }

    // если хотим одноразовый rebuild — можно раскомментировать
    // needRebuild = false;

    if (mainRectItem) { delete mainRectItem; mainRectItem = nullptr; }
    if (rightRectItem) { delete rightRectItem; rightRectItem = nullptr; }

    ingredientRows.clear();
    addIngredientBtn = nullptr;
    removeIngredientBtn = nullptr;

    Page::update_pages();
}

// ======================================================
//                 CREATE PANELS
// ======================================================

void PostPage::create_main_pannel()
{
    editMode();
}

void PostPage::create_right_pannel()
{
    // не используется базовым кодом, всё делаем в editMode
}

void PostPage::editMode()
{
    buildMainPanel();
    buildRightPanel();
}

// ======================================================
//                 MAIN PANEL
// ======================================================

void PostPage::buildMainPanel()
{
    const qreal margin  = 20;
    const qreal spacing = 10;

    if (mainRectItem) {
        delete mainRectItem;
        mainRectItem = nullptr;
    }

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
    titleEdit->setText("");

    y += titleEdit->boundingRect().height() + spacing;

    // CATEGORY
    categoryItem = new QGraphicsTextItem(mainRectItem);
    {
        QFont f; f.setPointSize(16);
        categoryItem->setFont(f);
        categoryItem->setDefaultTextColor(_scheme.groupColor());
        categoryItem->setTextWidth(mainRectItem->rect().width() - 2 * margin);
        categoryItem->setPlainText("Категория: Пользовательские");
        categoryItem->setPos(margin, y);
        y += categoryItem->boundingRect().height() + spacing;
    }

    // SAVE BUTTON
    saveBtn = new ButtonMew(_scheme, mainRectItem);
    saveBtn->setText("Создать");
    saveBtn->setPos(margin, y);

    connect(saveBtn, &ButtonMew::clicked, this, [=]() {
        saveRecipe();


    });

    y += saveBtn->boundingRect().height() + 30;

    // STEPS
    qreal stepsWidth = mainRectItem->rect().width() - 2 * margin;
    qreal stepsMinH  = 240;
    qreal stepsMaxH  = height * 0.6;

    stepsEdit = new TextEditMew(_scheme, mainRectItem);
    stepsEdit->setPos(margin, y);
    stepsEdit->setMaxWidth(stepsWidth);
    stepsEdit->setMinHeight(stepsMinH);
    stepsEdit->setMaxHeight(stepsMaxH);
    stepsEdit->setMultiline(true);
    stepsEdit->setAutoExpand(true);
    stepsEdit->setText("");
}

// ======================================================
//                 RIGHT PANEL
// ======================================================

void PostPage::buildRightPanel()
{
    if (rightRectItem) {
        delete rightRectItem;
        rightRectItem = nullptr;
    }

    qreal rightWidth = width - (width / 1.8 + leftRect.width());
    QRectF rightRect(0, 0, rightWidth, height);

    rightRectItem = new QGraphicsRectItem(rightRect, this);
    rightRectItem->setPos(width / 1.8 + leftRect.width() + 1, 0);
    rightRectItem->setBrush(_scheme.backgroundGet());
    rightRectItem->setPen(Qt::NoPen);

    // Заголовок
    auto* header = new QGraphicsTextItem("Ингредиенты", rightRectItem);
    QFont f; f.setPointSize(18); f.setBold(true);
    header->setFont(f);
    header->setDefaultTextColor(_scheme.additionalColorGet());
    header->setPos(20, 20);

    ingredientsTopY = header->boundingRect().height() + 40;

    // Первая строка
    buildIngredientsPanel(rightRectItem);

    // Кнопка "Добавить"
    addIngredientBtn = new ButtonMew(_scheme, rightRectItem);
    addIngredientBtn->setText("Добавить");

    connect(addIngredientBtn, &ButtonMew::clicked, this, [=]() {
        addIngredientRow();
    });

    // Кнопка "Удалить"
    removeIngredientBtn = new ButtonMew(_scheme, rightRectItem);
    removeIngredientBtn->setText("Удалить");

    connect(removeIngredientBtn, &ButtonMew::clicked, this, [=]() {
        removeLastIngredientRow();
    });

    repositionIngredientRows();
}

// ======================================================
//                 INGREDIENTS PANEL
// ======================================================

void PostPage::buildIngredientsPanel(QGraphicsItem* parent)
{
    ingredientRows.clear();

    QVector<Units> units = db->UnitsTable().Vector();

    IngredientRow* row = new IngredientRow(_scheme, units, parent);
    ingredientRows.append(row);

    repositionIngredientRows();
}

void PostPage::addIngredientRow()
{
    if (ingredientRows.size() >= 14)
        return;

    QVector<Units> units = db->UnitsTable().Vector();

    IngredientRow* row = new IngredientRow(_scheme, units, rightRectItem);
    ingredientRows.append(row);

    repositionIngredientRows();
}

void PostPage::removeLastIngredientRow()
{
    if (ingredientRows.size() <= 1)
        return;

    IngredientRow* row = ingredientRows.last();
    ingredientRows.removeLast();
    delete row;

    repositionIngredientRows();
}

void PostPage::repositionIngredientRows()
{
    qreal rowHeight = 45;
    qreal startX = 20;

    qreal y = ingredientsTopY;

    for (auto* row : ingredientRows)
    {
        row->setPos(startX, y);
        y += rowHeight;
    }

    if (addIngredientBtn)
        addIngredientBtn->setPos(startX, y + 10);

    if (removeIngredientBtn)
        removeIngredientBtn->setPos(startX + addIngredientBtn->boundingRect().width() + 10,
                                    y + 10);
}


// ======================================================
//                 SAVE
// ======================================================

void PostPage::saveRecipe()
{
    if (!db) return;

    QString title = titleEdit->text();
    QString steps = stepsEdit->text();

    QSqlDatabase dbsql = QSqlDatabase::database("cookbook_connection");
    if (!dbsql.isOpen()) return;

    int catId = getOrCreateUserCategory();

    QSqlQuery q(dbsql);
    q.prepare("INSERT INTO recipes (title, instructions, category_id) "
              "VALUES (:title, :steps, :cat)");
    q.bindValue(":title", title);
    q.bindValue(":steps", steps);
    q.bindValue(":cat", catId);

    if (!q.exec()) {
        qDebug() << "INSERT FAILED:" << q.lastError();
        return;
    }

    int newId = q.lastInsertId().toInt();
    _recipeID = newId;

    saveIngredients();

    emit goToRecipe();
}

void PostPage::saveIngredients()
{
    for (auto* row : ingredientRows)
    {
        QString name = row->ingredientName();
        if (name.trimmed().isEmpty())
            continue;

        double amount = row->amount().toDouble();
        int unitId = row->unitId();

        int ingId = findOrCreateIngredient(name);

        Recipeingred ri;
        ri.recipe_id     = _recipeID;
        ri.ingredient_id = ingId;
        ri.amount        = amount;
        ri.unit_id       = unitId;

        insertIngredientRow(ri);
    }
}

// ======================================================
//                 DB HELPERS
// ======================================================

int PostPage::findOrCreateIngredient(const QString& name)
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

void PostPage::insertIngredientRow(const Recipeingred& ri)
{
    db->RecipeIngredientsTable().Add(ri);
}





int PostPage::getOrCreateUserCategory()
{
    // 1. Ищем категорию
    for (const auto& cat : db->CategoriesTable().Vector())
    {
        if (cat.name.compare("Пользовательские", Qt::CaseInsensitive) == 0)
            return cat.id;
    }

    // 2. Если нет — создаём
    CategoriesRec  c;
    c.name = "Пользовательские";

    db->CategoriesTable().Add(c);
    db->CategoriesTable().Read();

    // 3. Ищем снова
    for (const auto& cat : db->CategoriesTable().Vector())
    {
        if (cat.name == "Пользовательские")
            return cat.id;
    }

    return 1; // fallback, но такого не будет
}
