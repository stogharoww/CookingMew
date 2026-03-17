#pragma once

#include "../Virtual/Button.h"

class ButtonMew : public Button
{
public:
    ButtonMew();

    ///
    /// \brief set_pixmap: Рисует картинку внутри кнопки
    /// \param path: путь до картинки
    ///
    void set_pixmap(QString& path) override;
    ///
    /// \brief set_color: устанавливает цвет кнопки
    /// \param col: Цвет в формате RGB
    ///
    void set_color(QColor& col) override;




private:


};
