#pragma once

#include "../Virtual/Page.h"
#include "../ColorScheme.h"

class ExplorePage : public Page
{
public:
    ExplorePage(ColorScheme &scheme, QRectF rect);

private:
    void create_main_pannel() override {};

    ColorScheme _scheme;
};
