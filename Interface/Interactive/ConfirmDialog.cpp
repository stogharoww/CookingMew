#include "ConfirmDialog.h"
#include <QPainter>

ConfirmDialog::ConfirmDialog(ColorScheme& scheme, QGraphicsItem* parent)
    : QGraphicsObject(parent), _scheme(scheme)
{
    // затемнение фона
    dimmer = new QGraphicsRectItem(this);
    dimmer->setBrush(QColor(0, 0, 0, 150));
    dimmer->setPen(Qt::NoPen);

    // окно
    box = new QGraphicsRectItem(this);
    box->setBrush(scheme.backgroundGet());
    box->setPen(scheme.borderGet());

    // текст
    message = new QGraphicsTextItem(box);
    QFont f; f.setPointSize(16);
    message->setFont(f);
    message->setDefaultTextColor(scheme.textColorGet());

    // кнопки
    yesBtn = new ButtonMew(scheme, box);
    yesBtn->setText("Да");

    noBtn = new ButtonMew(scheme, box);
    noBtn->setText("Нет");

    connect(yesBtn, &ButtonMew::clicked, this, [=]() {
        emit accepted();
        deleteLater();
    });

    connect(noBtn, &ButtonMew::clicked, this, [=]() {
        emit rejected();
        deleteLater();
    });
}

void ConfirmDialog::setMessage(const QString& text)
{
    message->setPlainText(text);

    qreal w = 400;
    qreal h = 200;

    // Центрируем окно
    box->setRect(0, 0, w, h);
    box->setPos(-w/2, -h/2);

    // ВНУТРИ box координаты начинаются с (0,0)
    message->setTextWidth(w - 40);
    message->setPos(20, 20);

    yesBtn->setPos(40, h - 60);
    noBtn->setPos(w - noBtn->boundingRect().width() - 40, h - 60);
}


QRectF ConfirmDialog::boundingRect() const
{
    return QRectF(-2000, -2000, 4000, 4000);
}
