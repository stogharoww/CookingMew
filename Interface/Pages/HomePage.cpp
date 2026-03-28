#include "HomePage.h"
#include "../Interactive/ButtonIcon.h"
#include <QTimer>


HomePage::HomePage(ColorScheme& scheme, QRectF rect)
    : Page(scheme, rect)
{


}

void HomePage::create_main_pannel()
{
    ButtonIcon *icon = new ButtonIcon(ButtonType::Like, scheme, this);
    icon->setPos(100, 100);
    icon->setParentItem(this);
}

