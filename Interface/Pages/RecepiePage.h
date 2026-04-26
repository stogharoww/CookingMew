#pragma once

#include "../Virtual/Page.h"
#include "../ColorScheme.h"

class RecepiePage : public Page
{
public:
    RecepiePage(ColorScheme &scheme, QRectF rect);
    void create_main_pannel() {};


private:
    ColorScheme _scheme;
};


