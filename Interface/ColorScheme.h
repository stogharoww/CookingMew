#pragma once
#include <QColor>
#include <QVector>
#include <QMap>

class ColorScheme
{
public:
    ColorScheme();
    ColorScheme(bool _nightTheme);
    QVector<QColor> getThemeColor();
    QVector<QColor> getThemeColor(bool nightTheme);
    void changeTheme();

    bool getColorScheme();
    void setColorScheme(bool night = true);



private:

    //night theme
    QColor baseColor = QColor("#800000");
    QColor secondColor = QColor("#0d0000");
    QColor hover = QColor("#990000");
    QColor pressed = QColor("#660000");
    QColor border = QColor("#4d0000");
    QColor background = QColor("#1a0000");

    //light theme
    QColor lightBase = QColor("#B00020");
    QColor lightSecond = QColor("#1a0000");
    QColor lhover = QColor("#C62828");
    QColor lPressed = QColor("#8E0000");
    QColor lBorder = QColor("#E57373");
    QColor lbackground = QColor("#ff9baa"); // Лососевый Крайола

    bool nightTheme = true;

};

