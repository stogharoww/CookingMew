#include "ColorScheme.h"
#include <QColor>

ColorScheme::ColorScheme() {


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
        background
    };

    QVector<QColor> colorDayTheme = {
        lightBase, lightSecond,
        lhover, lPressed, lBorder,
        lbackground
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

bool ColorScheme::getColorScheme()
{
    return nightTheme;
}

void ColorScheme::setColorScheme(bool night)
{
    nightTheme = night;
}

