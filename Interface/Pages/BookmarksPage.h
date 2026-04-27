#pragma once

#include "../Virtual/Page.h"
#include "../ColorScheme.h"

class BookmarksPage : public Page
{
public:
    BookmarksPage(ColorScheme &scheme, QRectF rect);

private:
    void create_main_pannel() override {};

    ColorScheme _scheme;
};
