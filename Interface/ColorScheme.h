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

    bool getBoolColorScheme();
    void setColorScheme(bool night = true);

    QColor baseColorGet();
    QColor secondColorGet();
    QColor hoverGet();
    QColor pressedGet();
    QColor borderGet();
    QColor backgroundGet();
    QColor textColorGet();
    QColor additionalColorGet();
    QColor groupColor();
    QColor titleColor();
    QColor tagColor();



private:

    //night theme
    QColor baseColor = QColor("#800000"); // основной цвет кнопки, красный
    QColor secondColor = QColor("#F5F5F5"); // текст (белый)
    QColor hover = QColor("#990000"); // мышка на кнопке
    QColor pressed = QColor("#660000"); // нажал на кнопку
    QColor border = QColor("#E57373"); // цвет линии
    QColor background = QColor("#1f1f1f"); // фон
    QColor textTheme = QColor("#F5F5F5"); // цвет текста вне кнопок (если надо)
    QColor groupTextTheme = QColor(Qt::magenta); //цвет названия групп (розовый)
    QColor titleTextTheme = QColor("#F08080"); // цвет названия рецепта (яркий коралл)
    QColor tagTextTheme = QColor("#890099"); // цвет тега (фиолетовый)


    QColor additionalBaseColor = QColor("#3cc8f8"); // дополнительный выделяющийся цвет приложения

    // Bright Pastel Pink Theme (high contrast)
    QColor lightBase      = QColor("#FF6EC7");   // яркая пастельно‑розовая кнопка (не кислотная)
    QColor lightSecond    = QColor("#000000");   // чёрный текст — максимальная читаемость
    QColor lhover         = QColor("#FF5ABF");   // hover — чуть темнее и насыщеннее
    QColor lPressed       = QColor("#E048A8");   // pressed — глубокий розовый
    QColor lBorder        = QColor("#FF9BDD");   // яркая пастельная граница
    QColor lbackground    = QColor("#FFD6E9");   // насыщенный пастельно‑розовый фон
    QColor ltextTheme     = QColor("#000000");   // чёрный текст вне кнопок

    QColor ladditionalBaseColor = QColor("#3cc8f8"); // дополнительный выделяющийся цвет приложения






    bool nightTheme = true;

};

