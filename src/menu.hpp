#pragma once

#include "graphicsHandler.hpp"

class Menu
{
    public:

        Menu() =  default;

        void linkMenu(GraphicsHandler* graphics);

        bool actOneInitiated(GraphicsHandler* graphics);

        bool actTwoInitiated(GraphicsHandler* graphics);

        bool actThreeInitiated(GraphicsHandler* graphics);

        bool isOn();

        bool gameEnded();

        bool menuInGameFlag();

        void drawButtons();

        void defaultAllButtons();

        void displayUpgrades();

        void upgradeMenu();

    private:

        bool inGame = false;

        GraphicsHandler* linkedGraphics;
    
        std::list<Button> buttons;
        std::list<Button> defaultButtons;
};