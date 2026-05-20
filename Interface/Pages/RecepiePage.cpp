#include "RecepiePage.h"

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFont>
#include <QFontMetricsF>#
#include <QDebug>
#include <QTextDocument>
#include "../Interactive/ConfirmDialog.h"


RecepiePage::RecepiePage(ColorScheme& scheme, QRectF rect)
    : Page(scheme, rect, PageID::recepie),
    _scheme(scheme)
{
}

void RecepiePage::update_pages()
{
    Page::update_pages();

    mainRectItem        = nullptr;
    rightRectItem       = nullptr;

    titleItem           = nullptr;
    categoryItem        = nullptr;
    stepsText           = nullptr;

    stepsBox            = nullptr;
    stepsScroll         = nullptr;

    editBtn             = nullptr;
    deleteBtn           = nullptr;

    ingredientsBox      = nullptr;
    ingredientsScroll   = nullptr;
    ingredientsViewText = nullptr;
    titleBox = nullptr;
    titleScroll = nullptr;

    // ===== ГРУЗИМ ДАННЫЕ ОДИН РАЗ =====

    _title.clear();
    _category.clear();
    _steps.clear();
    _ingredients.clear();

    if (_recipeID < 0)
        return;

    QSqlDatabase dbsql = QSqlDatabase::database("cookbook_connection");
    if (!dbsql.isOpen())
        return;

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

    if (q.exec() && q.next())
    {
        _title       = q.value("title").toString();
        _category    = q.value("category").toString();
        _steps       = q.value("instructions").toString();
        _ingredients = q.value("ingredients").toString();
    }

    qDebug() << "STEPS LEN =" << _steps.length();

}

QVector<QString> RecepiePage::getContent()
{
    return {_title, _category, _steps, _ingredients};
}

// ======================= ЛЕВАЯ ПАНЕЛЬ =======================

void RecepiePage::create_main_pannel()
{
    qreal leftWidth = width * 0.60;

    //QRectF mainRect(0, 0, leftWidth, height);
    QRectF mainRect = QRectF(0, 0, width / 1.8, height + 1);
    mainRectItem = new QGraphicsRectItem(mainRect, this);
    mainRectItem->setPos(leftRect.width(), 0);
    //mainRectItem->setPen(Qt::NoPen);
    mainRectItem->setPen(scheme.borderGet());
    mainRectItem->setBrush(scheme.backgroundGet());

    const qreal margin  = 20;
    const qreal spacing = 10;

    if (_recipeID < 0)
    {
        auto* msg = new QGraphicsTextItem("No recipe selected", mainRectItem);
        msg->setDefaultTextColor(Qt::red);
        msg->setPos(margin, margin);
        return;
    }

    if (_title.isEmpty())
    {
        auto* msg = new QGraphicsTextItem("Recipe not found", mainRectItem);
        msg->setDefaultTextColor(Qt::red);
        msg->setPos(margin, margin);
        return;
    }

    qreal y = margin;

    // ===== TITLE =====
    // ===== TITLE BLOCK =====
    qreal titleBoxWidth = mainRectItem->rect().width() -  2 * margin;
    qreal titleMinH     = 40;
    qreal titleMaxH     = 200;

    // === AutoGrowBox ===
    titleBox = new AutoGrowBox(titleBoxWidth, titleMinH, titleMaxH, mainRectItem);
    titleBox->setPos(margin, y);
    titleBox->setBorderVisible(false); // обычно заголовок без рамки
    //titleBox->setBorderColor(QColor("#444"));

    // === ScrollArea внутри AutoGrowBox ===
    titleScroll = new MewScrollArea(
        QRectF(0, 0, titleBoxWidth, titleBox->boundingRect().height()),
        titleBox
        );
    titleScroll->setPos(0, 0);

    // === TEXT ===
    titleItem = new QGraphicsTextItem();
    {
        QFont f;
        f.setPointSize(24);
        f.setBold(true);

        titleItem->setFont(f);
        titleItem->setDefaultTextColor(_scheme.titleColor());
        titleItem->setTextWidth(titleBoxWidth - 20); // небольшой padding
        titleItem->setPlainText(_title);
        titleItem->setPos(0, 0);

        // пересчёт layout
        titleItem->document()->adjustSize();
    }

    titleScroll->setContent(titleItem);
    titleScroll->updateContentGeometry();

    // === Авто‑рост AutoGrowBox ===
    qreal titleH = titleItem->document()->size().height() + 5;
    titleBox->setContentHeight(titleH);

    // === Синхронизация viewport ScrollArea ===
    titleScroll->setViewportHeight(titleBox->boundingRect().height());

    // обновляем Y для следующего блока
    y += titleBox->boundingRect().height() + spacing;


    // ===== CATEGORY =====
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

    // ===== BUTTONS =====
    editBtn = new ButtonMew(_scheme, mainRectItem);
    editBtn->setText("Редактировать");
    editBtn->setPos(margin, y);

    deleteBtn = new ButtonMew(_scheme, mainRectItem);
    deleteBtn->setText("Удалить рецепт");
    deleteBtn->setPos(editBtn->boundingRect().width() + 20, y);

    y += deleteBtn->boundingRect().height() + 20;


    // ===== STEPS =====
    qreal boxWidth = mainRectItem->rect().width() - 2 * margin;
    qreal boxMinH  = 120;
    qreal boxMaxH  = height * 0.6;

    // === AutoGrowBox ===
    stepsBox = new AutoGrowBox(boxWidth, boxMinH, boxMaxH, mainRectItem);
    stepsBox->setPos(margin, y);
    stepsBox->setBorderVisible(true);
    stepsBox->setBorderColor(QColor("#444"));

    // === ScrollArea внутри AutoGrowBox ===
    stepsScroll = new MewScrollArea(
        QRectF(0, 0, boxWidth, stepsBox->boundingRect().height()),
        stepsBox
        );
    stepsScroll->setPos(0, 0);

    // === TEXT ===
    stepsText = new QGraphicsTextItem();
    {
        QFont fSteps;
        fSteps.setPointSize(16);
        stepsText->setFont(fSteps);
        stepsText->setDefaultTextColor(_scheme.textColorGet());
        stepsText->setTextWidth(boxWidth - 40);

        stepsText->setPlainText(_steps);
        stepsText->setPos(0, 0);

        // заставляем QTextDocument пересчитать layout
        stepsText->document()->adjustSize();
    }

    stepsScroll->setContent(stepsText);
    stepsScroll->updateContentGeometry();

    // === Авто‑рост AutoGrowBox ===
    qreal contentH = stepsText->document()->size().height() + 5;
    stepsBox->setContentHeight(contentH);

    // === Синхронизация viewport ScrollArea ===
    stepsScroll->setViewportHeight(stepsBox->boundingRect().height());


    // QFontMetricsF fm(QFont("Arial", 14));
    // QStringList lines = _steps.split('\n');
    // if (lines.isEmpty()) lines << QString();
    // qreal textH = lines.size() * fm.height() + 2 * margin;
    // stepsBox->setContentHeight(boxMaxH);


    // stepsScroll->updateContentGeometry();


    //EDIT
    connect(editBtn, &ButtonMew::clicked, this, [=]() {
        emit changeCurrentPage(PageID::editPage);});


    // ===== DELETE =====
    connect(deleteBtn, &ButtonMew::clicked, this, [=]() {

        auto* dlg = new ConfirmDialog(_scheme, this);
        dlg->setZValue(9999);
        dlg->setPos(width/2, height/2);
        dlg->setMessage("Точно удалить рецепт?");

        connect(dlg, &ConfirmDialog::accepted, this, [=]() {

            // Удаляем ингредиенты
            QSqlDatabase dbsql = QSqlDatabase::database("cookbook_connection");
            QSqlQuery q(dbsql);

            q.prepare("DELETE FROM recipe_ingredients WHERE recipe_id = :id");
            q.bindValue(":id", _recipeID);
            q.exec();

            // Удаляем сам рецепт
            q.prepare("DELETE FROM recipes WHERE id = :id");
            q.bindValue(":id", _recipeID);
            q.exec();

            emit changeCurrentPage(PageID::home);
        });

        connect(dlg, &ConfirmDialog::rejected, this, []() {
            // ничего не делаем
        });
    });

}

// ======================= ПРАВАЯ ПАНЕЛЬ =======================

void RecepiePage::create_right_pannel()
{
    //qreal leftWidth  = width * 0.60;
    //qreal rightWidth = width - leftWidth - leftRect.width();
    qreal leftWidth = mainRectItem->boundingRect().width();
    qreal rightWidth = width - (mainRectItem->boundingRect().width() + leftRect.width());

    QRectF rightRect(0, 0, rightWidth, height);
    rightRectItem = new QGraphicsRectItem(rightRect, this);
    rightRectItem->setPos(leftRect.width() + leftWidth, 0);
    rightRectItem->setBrush(_scheme.backgroundGet());
    //rightRectItem->setPen(scheme.borderGet());
    rightRectItem->setPen(Qt::NoPen);

    // ===== HEADER =====
    auto* header = new QGraphicsTextItem("Ингредиенты", rightRectItem);
    QFont f; f.setPointSize(18); f.setBold(true);
    header->setFont(f);
    header->setDefaultTextColor(_scheme.additionalColorGet());
    header->setPos(20, 20);

    // ===== INGREDIENTS BLOCK =====
    qreal margin   = 20;
    qreal boxWidth = rightRect.width() - 2 * margin;
    qreal boxMinH  = 120;
    qreal boxMaxH  = height * 0.6;

    // === AutoGrowBox ===
    ingredientsBox = new AutoGrowBox(boxWidth, boxMinH, boxMaxH, rightRectItem);
    ingredientsBox->setPos(margin, header->boundingRect().height() + 2 * margin);
    ingredientsBox->setBorderVisible(true);
    ingredientsBox->setBorderColor(QColor("#444"));

    // === ScrollArea внутри AutoGrowBox ===
    ingredientsScroll = new MewScrollArea(
        QRectF(0, 0, boxWidth, ingredientsBox->boundingRect().height()),
        ingredientsBox
        );
    ingredientsScroll->setPos(0, 0);

    // === TEXT ===
    ingredientsViewText = new QGraphicsTextItem();
    {
        QFont f2; f2.setPointSize(14);
        ingredientsViewText->setFont(f2);
        ingredientsViewText->setDefaultTextColor(_scheme.textColorGet());
        ingredientsViewText->setTextWidth(boxWidth - 40);

        QStringList lines = _ingredients.split("\n");
        QString pretty;
        for (auto& l : lines)
            if (!l.trimmed().isEmpty())
                pretty += "• " + l + "\n";

        ingredientsViewText->setPlainText(pretty.trimmed());
        ingredientsViewText->setPos(0, 0);

        // заставляем QTextDocument пересчитать layout
        ingredientsViewText->document()->adjustSize();
    }

    ingredientsScroll->setContentItem(ingredientsViewText);
    ingredientsScroll->updateContentGeometry();

    // === Авто‑рост AutoGrowBox ===
    qreal contentH = ingredientsViewText->document()->size().height() + 5;
    ingredientsBox->setContentHeight(contentH);

    // === Синхронизация viewport ScrollArea ===
    ingredientsScroll->setViewportHeight(ingredientsBox->boundingRect().height());


}
