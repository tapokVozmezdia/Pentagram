#include "menu.hpp"

void Menu::linkMenu(GraphicsHandler* graphics)
{
    this->linkedGraphics = graphics;
    this->buttons.push_back({IdCounter::getFreeId(), "PLAY", true});
    this->buttons.push_back({IdCounter::getFreeId(), "STATS", true});
    this->buttons.push_back({IdCounter::getFreeId(), "QUIT", true});


    this->linkedGraphics->linkButtons(&(this->buttons));
}

bool Menu::isOn()
{
    for(auto it : this->buttons)
    {
        if (it.visible == true)
        {
            return true;
        }
    }
    return false;
}

void Menu::drawButtons()
{
    this->linkedGraphics->drawButtons();
}