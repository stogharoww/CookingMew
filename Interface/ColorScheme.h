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
    QColor baseColor = QColor("#800000"); // основной цвет кнопки
    QColor secondColor = QColor("#F5F5F5"); // текст
    QColor hover = QColor("#990000"); // мышка на кнопке
    QColor pressed = QColor("#660000"); // нажал на кнопку
    QColor border = QColor("#E57373"); // цвет линии
    QColor background = QColor("#1a0000"); // фон
    QColor textTheme = QColor("#F5F5F5"); // цвет текста вне кнопок (если надо)

    //light theme
    QColor lightBase = QColor("#B00020"); // основной цвет кнопки
    QColor lightSecond = QColor("#1a0000"); // текст
    QColor lhover = QColor("#C62828"); // мышка на кнопке
    QColor lPressed = QColor("#8E0000");  // нажал на кнопку
    QColor lBorder = QColor("#4d0000"); // цвет линии
    QColor lbackground = QColor("#ff9baa"); // Лососевый Крайола, фон
    QColor ltextTheme = QColor("#000000"); // цвет текста вне кнопок (если надо)

    bool nightTheme = true;

};

