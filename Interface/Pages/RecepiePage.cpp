#include "RecepiePage.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFont>
#include <QDebug>

RecepiePage::RecepiePage(ColorScheme& scheme, QRectF rect)
    : Page(scheme, rect, PageID::recepie),
    _scheme(scheme)
{
}

void RecepiePage::create_main_pannel()
{
    QRectF mainRect = QRectF(0, 0, width / 1.8, height);
    mainRectItem = new QGraphicsRectItem(mainRect, this);
    mainRectItem->setPos(leftRect.width(), 0);
    mainRectItem->setPen(QPen(scheme.borderGet(), 1));
    mainRectItem->setBrush(scheme.backgroundGet());

    if (_recipeID < 0)
    {
        auto* msg = new QGraphicsTextItem("No recipe selected", mainRectItem);
        msg->setDefaultTextColor(QColor("#ff6666"));
        QFont f; f.setPointSize(20); f.setBold(true);
        msg->setFont(f);
        msg->setPos(20, 20);
        return;
    }

    QSqlDatabase dbsql = QSqlDatabase::database("cookbook_connection");
    if (!dbsql.isOpen())
    {
        auto* msg = new QGraphicsTextItem("DB not open!", mainRectItem);
        msg->setDefaultTextColor(QColor("#ff6666"));
        QFont f; f.setPointSize(20); f.setBold(true);
        msg->setFont(f);
        msg->setPos(20, 20);
        return;
    }

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

    QSqlQuery q(dbsql);
    q.prepare(sql);
    q.bindValue(":id", _recipeID);

    if (!q.exec() || !q.next())
    {
        auto* msg = new QGraphicsTextItem("Recipe not found", mainRectItem);
        msg->setDefaultTextColor(QColor("#ff6666"));
        QFont f; f.setPointSize(20); f.setBold(true);
        msg->setFont(f);
        msg->setPos(20, 20);
        return;
    }

    QString title       = q.value("title").toString();
    QString category    = q.value("category").toString();
    QString ingredients = q.value("ingredients").toString();
    QString steps       = q.value("instructions").toString();

    _ingredientsRaw = ingredients;

    QRectF scrollRect(0, 0, mainRect.width(), mainRect.height());
    scrollArea = new MewScrollArea(scrollRect, mainRectItem);
    scrollArea->setPos(0, 0);

    auto* root = new ContentRoot();
    scrollArea->setContent(root);

    buildContent(root, title, category, ingredients, steps);
}

void RecepiePage::buildContent(ContentRoot* root,
                               const QString& title,
                               const QString& category,
                               const QString& ingredients,
                               const QString& steps)
{
    const qreal margin  = 20;
    const qreal spacing = 14;
    const qreal contentW = (width / 1.8) - 2 * margin;

    qreal y = margin;

    // === TITLE ===
    auto* titleItem = new QGraphicsTextItem(root);
    {
        QFont f; f.setPointSize(24); f.setBold(true);
        titleItem->setFont(f);
        titleItem->setDefaultTextColor(_scheme.titleColor());
        titleItem->setTextWidth(contentW);
        titleItem->setPlainText(title);
        titleItem->setPos(margin, y);
        y += titleItem->boundingRect().height() + spacing;
    }

    // === CATEGORY ===
    auto* categoryItem = new QGraphicsTextItem(root);
    {
        QFont f; f.setPointSize(16);
        categoryItem->setFont(f);
        categoryItem->setDefaultTextColor(_scheme.groupColor());
        categoryItem->setTextWidth(contentW);
        categoryItem->setPlainText("Категория: " + category);
        categoryItem->setPos(margin, y);
        y += categoryItem->boundingRect().height() + spacing;
    }

    // === STEPS HEADER ===
    auto* stepsHeader = new QGraphicsTextItem(root);
    {
        QFont f; f.setPointSize(16); f.setBold(true);
        stepsHeader->setFont(f);
        stepsHeader->setDefaultTextColor(_scheme.textColorGet());
        stepsHeader->setTextWidth(contentW);
        stepsHeader->setPlainText("Этапы приготовления:");
        stepsHeader->setPos(margin, y);
        y += stepsHeader->boundingRect().height() + spacing;
    }

    // === STEPS TEXT ===
    auto* stepsText = new QGraphicsTextItem(root);
    {
        QFont f; f.setPointSize(14);
        stepsText->setFont(f);
        stepsText->setDefaultTextColor(_scheme.textColorGet());
        stepsText->setTextWidth(contentW);
        stepsText->setPlainText(steps);
        stepsText->setPos(margin, y);
        y += stepsText->boundingRect().height() + spacing;
    }

    // === EDIT FIELDS (только название + шаги) ===
    auto* titleEdit = new TextEditMew(_scheme, root);
    titleEdit->setText(title);
    titleEdit->setMultiline(false);
    titleEdit->setAutoExpand(false);
    titleEdit->setMinHeight(40);
    titleEdit->setMaxWidth(contentW);
    titleEdit->setVisible(false);
    titleEdit->setPos(margin, titleItem->pos().y());

    auto* stepsEdit = new TextEditMew(_scheme, root);
    stepsEdit->setText(steps);
    stepsEdit->setMultiline(true);
    stepsEdit->setAutoExpand(true);
    stepsEdit->setMinHeight(120);
    stepsEdit->setMaxWidth(contentW);
    stepsEdit->setVisible(false);
    stepsEdit->setPos(margin, stepsText->pos().y());

    // === BUTTONS ===
    auto* editBtn = new ButtonMew(_scheme, root);
    editBtn->setText("Редактировать");
    editBtn->setPos(margin, y);

    auto* saveBtn = new ButtonMew(_scheme, root);
    saveBtn->setText("Сохранить");
    saveBtn->setPos(margin + 160, y);
    saveBtn->setVisible(false);

    auto* deleteBtn = new ButtonMew(_scheme, root);
    deleteBtn->setText("Удалить рецепт");
    deleteBtn->setPos(margin + 320, y);

    y += 60;

    root->setBounds(QRectF(0, 0, contentW, y));

    // === EDIT MODE ===
    connect(editBtn, &ButtonMew::clicked, this, [=]() {
        _editMode = true;

        titleItem->setVisible(false);
        stepsText->setVisible(false);

        titleEdit->setVisible(true);
        stepsEdit->setVisible(true);

        editBtn->setVisible(false);
        saveBtn->setVisible(true);

        create_right_pannel();
    });

    // === SAVE ===
    connect(saveBtn, &ButtonMew::clicked, this, [=]() {
        if (!db) return;

        QString newTitle = titleEdit->text();
        QString newSteps = stepsEdit->text();
        QString newIngredients = ingredientsEdit ? ingredientsEdit->text() : _ingredientsRaw;

        Recipes r;
        r.id           = _recipeID;
        r.title        = newTitle;
        r.instructions = newSteps;
        r.category_id  = 1;

        if (db->RecipesTable().Update(r))
        {
            _ingredientsRaw = newIngredients;

            titleItem->setPlainText(newTitle);
            stepsText->setPlainText(newSteps);

            titleItem->setVisible(true);
            stepsText->setVisible(true);

            titleEdit->setVisible(false);
            stepsEdit->setVisible(false);

            saveBtn->setVisible(false);
            editBtn->setVisible(true);

            _editMode = false;
            create_right_pannel();
        }
    });

    // === DELETE ===
    connect(deleteBtn, &ButtonMew::clicked, this, [=]() {
        if (!db) return;

        for (auto& ri : db->RecipeIngredientsTable().Vector())
            if (ri.recipe_id == _recipeID)
                db->RecipeIngredientsTable().Delete(ri);

        Recipes rec;
        rec.id = _recipeID;

        if (db->RecipesTable().Delete(rec))
            emit changeCurrentPage(PageID::home);
    });
}

void RecepiePage::create_right_pannel()
{
    QRectF rightRect = QRectF(0, 0,
                              width - (width / 1.8) - leftRect.width(),
                              height);

    rightRectItem = new QGraphicsRectItem(rightRect, this);
    rightRectItem->setPos(leftRect.width() + width / 1.8, 0);
    rightRectItem->setPen(QPen(_scheme.borderGet(), 1));
    rightRectItem->setBrush(_scheme.backgroundGet());

    auto* header = new QGraphicsTextItem("Ингредиенты", rightRectItem);
    QFont f; f.setPointSize(18); f.setBold(true);
    header->setFont(f);
    header->setDefaultTextColor(_scheme.additionalColorGet());
    header->setPos(20, 20);

    if (!_editMode)
    {
        auto* list = new QGraphicsTextItem(rightRectItem);
        QFont f2; f2.setPointSize(14);
        list->setFont(f2);
        list->setDefaultTextColor(_scheme.textColorGet());
        list->setTextWidth(rightRect.width() - 40);

        QStringList lines = _ingredientsRaw.split("\n");
        QString pretty;
        for (auto& l : lines)
            pretty += "• " + l + "\n";

        list->setPlainText(pretty.trimmed());
        list->setPos(20, 60);
    }
    else
    {
        ingredientsEdit = new TextEditMew(_scheme, rightRectItem);
        ingredientsEdit->setText(_ingredientsRaw);
        ingredientsEdit->setMultiline(true);
        ingredientsEdit->setAutoExpand(true);
        ingredientsEdit->setMinHeight(200);
        ingredientsEdit->setMaxWidth(rightRect.width() - 40);
        ingredientsEdit->setPos(20, 60);

        rightRectItem->setPen(QPen(_scheme.additionalColorGet(), 3));
    }
}
