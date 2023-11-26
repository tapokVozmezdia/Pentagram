#pragma once

#include "graphicsHandler.hpp"

class Menu
{
    public:

        Menu() =  default;

        void linkMenu(GraphicsHandler* graphics);

        bool isOn();

        void drawButtons();

    private:

        GraphicsHandler* linkedGraphics;

        std::list<Button> buttons;
};