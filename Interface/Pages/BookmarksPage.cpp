#include "BookmarksPage.h"


BookmarksPage::BookmarksPage(ColorScheme &scheme, QRectF rect)
    : Page(scheme, rect, PageID::bookmarks),
    _scheme(scheme)
{

}
