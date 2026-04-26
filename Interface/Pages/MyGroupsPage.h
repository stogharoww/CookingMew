#pragma once

#include "../Virtual/Page.h"
#include "../ColorScheme.h"

class MyGroupsPage : public Page
{
public:
    MyGroupsPage(ColorScheme &scheme, QRectF rect);

private:
    void create_main_pannel() override {};

    ColorScheme _scheme;
};
