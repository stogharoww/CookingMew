#include "RecepiePage.h"


RecepiePage::RecepiePage(ColorScheme &scheme, QRectF rect)
    : Page(scheme, rect),
    _scheme(scheme)
{
    change_current_btm(textForBtm[1]);
    currentBtm = textForBtm[1];
}

