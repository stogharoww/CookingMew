#pragma once

#include "../Virtual/Page.h"
#include "../ColorScheme.h"

class IngredientsPage : public Page
{
public:
    IngredientsPage(ColorScheme &scheme, QRectF rect);

private:
    void create_main_pannel() override {};

    ColorScheme _scheme;
};
