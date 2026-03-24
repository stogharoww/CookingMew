#include "ButtonIcon.h"



ButtonIcon::ButtonIcon(ButtonType type, ColorScheme &scheme, QColor mainColor, QGraphicsItem *paren)
    : Button(scheme, paren),
    mainCol(mainColor),
    colors(scheme.getThemeColor()),
    btmType(type)
{

}
