#include "menu.hpp"

void Menu::linkMenu(GraphicsHandler* graphics)
{
    this->linkedGraphics = graphics;
    this->buttons.push_back({IdCounter::getFreeId(), "PLAY", true});
    this->buttons.push_back({IdCounter::getFreeId(), "STATS", true});
    this->buttons.push_back({IdCounter::getFreeId(), "QUIT", true});
    this->buttons.push_back({IdCounter::getFreeId(), "LEVELS", false});
    this->buttons.push_back({IdCounter::getFreeId(), "DIFFICULTY", false});
    this->buttons.push_back({IdCounter::getFreeId(), "LEVEL 1", false});
    this->buttons.push_back({IdCounter::getFreeId(), "LEVEL 2", false});
    this->buttons.push_back({IdCounter::getFreeId(), "LEVEL 3", false});
    this->buttons.push_back({IdCounter::getFreeId(), "LEVEL 4", false});
    this->buttons.push_back({IdCounter::getFreeId(), "BACK", false});


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