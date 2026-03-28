#include "ButtonIcon.h"
#include <QtSvg/QSvgRenderer>
#include <QPainter>



ButtonIcon::ButtonIcon(ButtonType type, ColorScheme &scheme, QGraphicsItem *paren, QColor mainColor)
    : Button(scheme, paren),
    //mainCol(mainColor),
    colors(scheme.getThemeColor()),
    btmType(type)
{
    if (mainColor != nullptr)
        mainCol = mainColor;
    if (mainColor == nullptr)
        mainCol = scheme.getThemeColor()[7];

    iconAppend();
}

QString ButtonIcon::iconPath()
{
    switch (btmType){
    case ButtonType::Like: return ":/icons/resourses/icons/heart.svg";
    case ButtonType::Comment: return ":/icons/resourses/icons/comment.svg";
    case ButtonType::Bookmarks: return ":/icons/resourses/icons/bookmark.svg";
    case ButtonType::AddBtm: return ":/icons/resourses/icons/plus.svg";
    case ButtonType::Expand: return ":/icons/resourses/icons/expand.svg";
    }
}

void ButtonIcon::iconAppend()
{
    pathToIcons = iconPath();

    QSvgRenderer svg(pathToIcons);

    QPixmap pix(40, 40);
    pix.fill(Qt::transparent);

    // Рендерим SVG в pixmap
    {
        QPainter p1(&pix);
        svg.render(&p1);
    }

    // Перекрашиваем
    {
        QPainter p2(&pix);
        p2.setCompositionMode(QPainter::CompositionMode_SourceIn);
        p2.fillRect(pix.rect(), mainCol);
    }

    iconPixmap = pix;   // сохраняем результат
    update();           // просим Qt перерисовать элемент
}

void ButtonIcon::paint(QPainter* painter,
                       const QStyleOptionGraphicsItem*,
                       QWidget*)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    // Рисуем фон кнопки (если нужно)
    painter->setPen(Qt::NoPen);
    painter->setBrush(mainCol);
    painter->drawRect(boundingRect());

    // Рисуем иконку
    if (!iconPixmap.isNull()) {

        QRectF br = boundingRect();

        QRectF target(
            br.center().x() - iconPixmap.width() / 2,
            br.center().y() - iconPixmap.height() / 2,
            iconPixmap.width(),
            iconPixmap.height()
            );

        QRectF source(0, 0, iconPixmap.width(), iconPixmap.height());

        painter->drawPixmap(target, iconPixmap, source);
    }

}

void ButtonIcon::set_pixmap(QString &path)
{

}



