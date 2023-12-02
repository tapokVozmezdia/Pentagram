#include "menu.hpp"

void Menu::linkMenu(GraphicsHandler* graphics)
{
    this->linkedGraphics = graphics;
    this->buttons.push_back({IdCounter::getFreeId(), "PLAY", true});
    this->buttons.push_back({IdCounter::getFreeId(), "SETTINGS", true});
    this->buttons.push_back({IdCounter::getFreeId(), "STATS", true});
    this->buttons.push_back({IdCounter::getFreeId(), "QUIT", true});
    this->buttons.push_back({IdCounter::getFreeId(), "LEVELS", false});
    this->buttons.push_back({IdCounter::getFreeId(), "DIFFICULTY", false});
    this->buttons.push_back({IdCounter::getFreeId(), "LEVEL 1", false});
    this->buttons.push_back({IdCounter::getFreeId(), "LEVEL 2", false});
    this->buttons.push_back({IdCounter::getFreeId(), "LEVEL 3", false});
    this->buttons.push_back({IdCounter::getFreeId(), "LEVEL 4", false});
    this->buttons.push_back({IdCounter::getFreeId(), "BACK", false});

    
    this->defaultButtons = this->buttons;

    this->linkedGraphics->linkButtons(&(this->buttons));
}

bool Menu::actOneInitiated(GraphicsHandler* graphics)
{
    for(auto it : this->buttons)
    {
        if (it.text == "LEVEL 1" && it.is_clicked == true)
        {
            this->inGame = true;
            graphics->flagTheStart();
            return true;
        }
    }
    return false;
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

bool Menu::menuInGameFlag()
{
    return this->inGame;
}

bool Menu::gameEnded()
{
    return !(this->linkedGraphics->checkPulse());
}

void Menu::drawButtons()
{
    this->linkedGraphics->drawButtons();
}

void Menu::defaultAllButtons()
{
    this->inGame = false;
    auto jt = this->defaultButtons.begin();
    for (auto it = this->buttons.begin(); it != this->buttons.end(); ++it)
    {
        (*it).visible = (*jt).visible;
        jt++;
    }
    this->linkedGraphics->clearAll();
}