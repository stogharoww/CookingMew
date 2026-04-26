#include "FollowLent.h"
#include <QPainter>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

FollowLent::FollowLent(ColorScheme& scheme, QRectF rect, QGraphicsItem* parent)
    : QGraphicsObject(parent),
    _scheme(scheme),
    globalRect(rect)
{
    QSqlDatabase db = QSqlDatabase::database("cookbook_connection");

    if (!db.isOpen()) {
        qDebug() << "DB not open!";
        return;
    }

    // Полный SQL-запрос, собирающий всё, что нужно
    QString sql =
        "SELECT r.id, r.title, r.instructions, c.name AS category, "
        "GROUP_CONCAT(i.name || ' — ' || ri.amount || ' ' || u.short_name, '\n') AS ingredients "
        "FROM recipes r "
        "JOIN categories c ON r.category_id = c.id "
        "LEFT JOIN recipe_ingredients ri ON ri.recipe_id = r.id "
        "LEFT JOIN ingredients i ON i.id = ri.ingredient_id "
        "LEFT JOIN units u ON u.id = ri.unit_id "
        "GROUP BY r.id "
        "ORDER BY r.id;";

    QSqlQuery q(db);
    if (!q.exec(sql)) {
        qDebug() << "SQL error:" << q.lastError().text();
        return;
    }

    qreal spacing = 4;
    qreal y = globalRect.top();

    while (q.next()) {

        QString title       = q.value("title").toString();
        QString group       = q.value("category").toString();
        QString ingredients = q.value("ingredients").toString();
        QString steps       = q.value("instructions").toString();
        QString tag         = "Database"; // пока нет тегов — оставляем пустым
        int id = q.value("id").toInt();

        // Создаём карточку
        MewItem* post = new MewItem(_scheme, globalRect, this);

        // Передаём данные
        post->setRecepieID(id);
        post->setContent(title, group, ingredients, steps, tag);

        // Позиционируем
        post->setPos(0, y);

        posts.append(post);


        // Увеличиваем y на высоту карточки + отступ
        y += post->boundingRect().height() + spacing;
        connect(post, &MewItem::clicked, this, [this](int id){
            qDebug() << "[FollowLent] got clicked from MewItem, id =" << id;
            emit openRecepie(id);
        });

    }



    // Границы ленты
    _bounds = QRectF(0, 0, globalRect.width(), y);
}



QRectF FollowLent::boundingRect() const
{
    return _bounds;
}

void FollowLent::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)
{
    // Лента сама ничего не рисует — всё делают MewItem
}

void FollowLent::addPost(MewItem* post)
{
    if (!post)
        return;

    const qreal spacing = 4; // отступы

    qreal y = globalRect.top();
    if (!posts.isEmpty()) {
        MewItem* last = posts.last();
        y = last->pos().y() + globalRect.height() + spacing;
    }

    post->setParentItem(this);
    post->setPos(globalRect.left(), y);
    posts.append(post);

    prepareGeometryChange();
    _bounds.setHeight((posts.size()) * globalRect.height());
}
