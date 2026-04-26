cmake_minimum_required(VERSION 3.16)

project(CookingMew VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Svg)


set(PROJECT_SOURCES
        main.cpp



)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

    qt_add_executable(CookingMew
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}


        Database/Models.h
        Database/database.h Database/database.cpp
        Database/DatabaseCore.h



        Interface/Interactive/ButtonMew.cpp Interface/Items/IngridientsList.cpp Interface/Pages/HomePage.cpp Interface/Virtual/Button.h Interface/Virtual/Page.h
        Interface/Interactive/ButtonMew.cpp Interface/Items/IngridientsList.cpp Interface/Pages/HomePage.cpp Interface/Virtual/Button.h Interface/Virtual/Page.h
        Interface/Items/IngridientsList.cpp
        Interface/Interactive/ButtonMew.h Interface/Interactive/ButtonMew.cpp
        Interface/Pages/HomePage.h Interface/Pages/HomePage.cpp
        Interface/Items/IngridientsList.h Interface/Items/IngridientsList.cpp
        Mew.h Mew.cpp
        Interface/Virtual/Button.cpp
        Interface/ColorScheme.h Interface/ColorScheme.cpp
        Interface/Virtual/Page.cpp
        Interface/Items/Line.h Interface/Items/Line.cpp
        res.qrc
        Database/categoriesrec.cpp Database/categoriesrec.h Database/database.cpp Database/database.h Database/DatabaseCore.h Database/ingredients.cpp Database/ingredients.h Database/Models.h Database/recipeingred.cpp Database/recipeingred.h Database/recipes.cpp Database/recipes.h Database/units.cpp Database/units.h
        Interface/Interactive/MewItem.h Interface/Interactive/MewItem.cpp
        Interface/Interactive/ButtonIcon.h Interface/Interactive/ButtonIcon.cpp
        Interface/Interactive/SquareTextBtm.h Interface/Interactive/SquareTextBtm.cpp
        Interface/Interactive/MewScrollArea.h Interface/Interactive/MewScrollArea.cpp
        Interface/Items/FollowLent.h Interface/Items/FollowLent.cpp
        Interface/Pages/RecepiePage.h Interface/Pages/RecepiePage.cpp
        Logic/Logic.h
        Logic/PageChanger.h Logic/PageChanger.cpp

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CookingMew APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CookingMew SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CookingMew
            ${PROJECT_SOURCES}
            Database/database.cpp Database/database.h Database/DatabaseCore.h Database/Models.h
        )
    endif()
endif()

target_link_libraries(CookingMew PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Svg
    Qt${QT_VERSION_MAJOR}::Core
)
target_link_libraries(CookingMew PRIVATE Qt6::Core)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CookingMew)
endif()
set_target_properties(CookingMew PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CookingMew
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CookingMew)
endif()
