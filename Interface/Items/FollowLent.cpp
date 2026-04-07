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
    //Подключаем к базе данных
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "cookbook_connection");
    db.setDatabaseName("CookBook(3).db");

    if(!db.open())
    {
        return;
    };

    QSqlQuery q(db);
    if(!q.exec("SELECT id, title FROM recipes"))
    {
        return;
    };

   /* const int count = 3;
    qreal spacing = 4;
    qreal y = globalRect.top();

    for (int i = 0; i < count; ++i) {
        MewItem* post = new MewItem(_scheme, globalRect, this);

        qreal postHeight = post->boundingRect().height();

        post->setPos(0, y);
        posts.append(post);

        y += postHeight + spacing;


    }

    // Высота ленты = count * globalRect.height()
    _bounds = QRectF(0, 0, globalRect.width(), y);
    */

    qreal spacing = 4;
    qreal y = globalRect.top();

    while(q.next())
    {
        int id = q.value(0).toInt();
        QString title = q.value(1).toString();

        MewItem* post = new MewItem(_scheme, globalRect, this);
        post->setTitle(title);
        post->setPos(0, y);
        posts.append(post);

        y += post->boundingRect().height() + spacing;

    }
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
