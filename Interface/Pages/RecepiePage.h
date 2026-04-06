#pragma once

#include "../Virtual/Page.h"
#include "../ColorScheme.h"

class RecepiePage : public Page
{
public:
    RecepiePage(ColorScheme &scheme, QRectF rect);

private:
    ColorScheme _scheme;
};


