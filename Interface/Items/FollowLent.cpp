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

    QSqlQuery q(db);
    if (!q.exec("SELECT id, title FROM recipes")) {
        qDebug() << "SQL error:" << q.lastError().text();
        return;
    }

    qreal spacing = 4;
    qreal y = globalRect.top();

    while (q.next()) {
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
