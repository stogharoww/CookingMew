#include "RecepiePage.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QGraphicsProxyWidget>
#include <QDebug>

RecepiePage::RecepiePage(ColorScheme &scheme, QRectF rect)
    : Page(scheme, rect, PageID::recepie),
    _scheme(scheme)
{
}

void RecepiePage::create_main_pannel()
{
    // Удаляем старую панель, если была
    if (mainRectItem) {
        delete mainRectItem;
        mainRectItem = nullptr;
    }

    // ====== ПРОКСИ + SCROLL ======
    QScrollArea* scroll = new QScrollArea();
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setFrameShape(QFrame::NoFrame);

    scroll->setStyleSheet(
        "QScrollArea { background: transparent; }"
        "QWidget#content { background-color: #202020; }"
        "QScrollBar:vertical { background: #202020; width: 10px; }"
        "QScrollBar::handle:vertical { background: #505050; border-radius: 4px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
        );

    QWidget* content = new QWidget();
    content->setObjectName("content");

    QVBoxLayout* layout = new QVBoxLayout(content);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(16);

    scroll->setWidget(content);
    scroll->setWidgetResizable(true);

    // Встраиваем в сцену
    QGraphicsProxyWidget* proxy = new QGraphicsProxyWidget(this);
    proxy->setWidget(scroll);
    proxy->setPos(leftRect.width(), 0);
    proxy->setMinimumWidth(width / 1.8);
    proxy->setMaximumWidth(width / 1.8);
    proxy->setMinimumHeight(height);
    proxy->setMaximumHeight(height);

    // ====== ЕСЛИ ID НЕ ЗАДАН ======
    if (_recipeID < 0)
    {
        QLabel* noID = new QLabel("No recipe selected");
        noID->setStyleSheet("color: #ff6666; font-size: 20px; font-weight: bold;");
        layout->addWidget(noID);
        layout->addStretch();
        return;
    }

    // ====== БАЗА ======
    QSqlDatabase db = QSqlDatabase::database("cookbook_connection");
    if (!db.isOpen())
    {
        QLabel* err = new QLabel("DB not open!");
        err->setStyleSheet("color: #ff6666; font-size: 20px; font-weight: bold;");
        layout->addWidget(err);
        layout->addStretch();
        return;
    }

    // ====== SQL ======
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
        QLabel* err = new QLabel("Recipe not found");
        err->setStyleSheet("color: #ff6666; font-size: 20px; font-weight: bold;");
        layout->addWidget(err);
        layout->addStretch();
        return;
    }

    QString title       = q.value("title").toString();
    QString category    = q.value("category").toString();
    QString ingredients = q.value("ingredients").toString();
    QString steps       = q.value("instructions").toString();

    // ===== TITLE =====
    QLabel* titleLabel = new QLabel(title);
    titleLabel->setWordWrap(true);
    titleLabel->setStyleSheet(
        "color: " + _scheme.titleColor().name() + ";"
                                                  "font-size: 26px;"
                                                  "font-weight: bold;"
        );
    layout->addWidget(titleLabel);

    // ===== CATEGORY =====
    QLabel* categoryLabel = new QLabel("Category: " + category);
    categoryLabel->setStyleSheet(
        "color: " + _scheme.groupColor().name() + ";"
                                                  "font-size: 18px;"
        );
    layout->addWidget(categoryLabel);

    // ===== LINE =====
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #404040;");
    layout->addWidget(line);

    // ===== INGREDIENTS =====
    QLabel* ingHeader = new QLabel("Ingredients:");
    ingHeader->setStyleSheet(
        "color: " + _scheme.additionalColorGet().name() + ";"
                                                          "font-size: 16px;"
                                                          "font-weight: bold;"
        );
    layout->addWidget(ingHeader);

    QLabel* ingredientsLabel = new QLabel(ingredients);
    ingredientsLabel->setWordWrap(true);
    ingredientsLabel->setStyleSheet(
        "color: " + _scheme.additionalColorGet().name() + ";"
                                                          "font-size: 14px;"
        );
    layout->addWidget(ingredientsLabel);

    // ===== STEPS =====
    QLabel* stepsHeader = new QLabel("Steps:");
    stepsHeader->setStyleSheet(
        "color: " + _scheme.textColorGet().name() + ";"
                                                    "font-size: 16px;"
                                                    "font-weight: bold;"
        );
    layout->addWidget(stepsHeader);

    QLabel* stepsLabel = new QLabel(steps);
    stepsLabel->setWordWrap(true);
    stepsLabel->setStyleSheet(
        "color: " + _scheme.textColorGet().name() + ";"
                                                    "font-size: 14px;"
        );
    layout->addWidget(stepsLabel);

    layout->addStretch();
}