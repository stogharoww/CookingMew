#include "ColorScheme.h"
#include <QColor>

ColorScheme::ColorScheme()
{

}

ColorScheme::ColorScheme(bool _nightTheme)
{
    nightTheme = _nightTheme;
}

QVector<QColor> ColorScheme::getThemeColor()
{
    QVector<QColor> colorNightTheme = {
        baseColor, secondColor,
        hover, pressed, border,
        background,
        textTheme, additionalBaseColor
    };

    QVector<QColor> colorDayTheme = {
        lightBase, lightSecond,
        lhover, lPressed, lBorder,
        lbackground,
        ltextTheme, ladditionalBaseColor
    };

    if (!nightTheme)
        return colorDayTheme;

    return colorNightTheme;

}

QVector<QColor> ColorScheme::getThemeColor(bool _nightTheme)
{
    bool tmp = nightTheme;
    nightTheme = _nightTheme;
    QVector<QColor> colorTheme = getThemeColor();
    nightTheme = tmp;
    return colorTheme;
}

void ColorScheme::changeTheme()
{
    nightTheme = !nightTheme;
}

bool ColorScheme::getBoolColorScheme()
{
    return nightTheme;
}

void ColorScheme::setColorScheme(bool night)
{
    nightTheme = night;
}

QColor ColorScheme::baseColorGet()
{
    return getThemeColor()[0];
}

QColor ColorScheme::secondColorGet()
{
    return getThemeColor()[1];
}

QColor ColorScheme::hoverGet()
{
    return getThemeColor()[2];
}

QColor ColorScheme::pressedGet()
{
    return getThemeColor()[3];
}

QColor ColorScheme::borderGet()
{
    return getThemeColor()[4];
}

QColor ColorScheme::backgroundGet()
{
    return getThemeColor()[5];
}

QColor ColorScheme::textColorGet()
{
    return getThemeColor()[6];
}

QColor ColorScheme::additionalColorGet()
{
    return getThemeColor()[7];
}



