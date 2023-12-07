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
    this->buttons.push_back({IdCounter::getFreeId(), "EASY", false});
        (this->buttons.back()).isDifficulty = true;
    this->buttons.push_back({IdCounter::getFreeId(), "NORMAL", false});
        (this->buttons.back()).isDifficulty = true;
    this->buttons.push_back({IdCounter::getFreeId(), "HARD", false});
        (this->buttons.back()).isDifficulty = true;
    this->buttons.push_back({IdCounter::getFreeId(), "LEVEL 1", false});
    this->buttons.push_back({IdCounter::getFreeId(), "LEVEL 2", false});
    this->buttons.push_back({IdCounter::getFreeId(), "LEVEL 3", false});
    this->buttons.push_back({IdCounter::getFreeId(), "LEVEL 4", false});
    this->buttons.push_back({IdCounter::getFreeId(), "BACK", false});

    Button u1{IdCounter::getFreeId(), "DAMAGE", false};
    Button u2{IdCounter::getFreeId(), "HEALTH", false};
    Button u3{IdCounter::getFreeId(), "SHIELD", false};
    
    u1.isUpgrade = true;
    u2.isUpgrade = true;
    u3.isUpgrade = true;
    u1.upgradeText = "Gives +" + std::to_string(AT_BUFF) + " dmg";
    u2.upgradeText = "Gives +" + std::to_string(HP_BUFF) + " hp";
    u3.upgradeText = "Gives +" + std::to_string(SH_BUFF) + " shield";

    u1.height = 100;
    u1.width = 100;
    u2.height = 100;
    u2.width = 100;
    u3.height = 100;
    u3.width = 100;

    this->buttons.push_back(u1);
    this->buttons.push_back(u2);
    this->buttons.push_back(u3);

    
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


bool Menu::actTwoInitiated(GraphicsHandler* graphics)
{
    for(auto it : this->buttons)
    {
        if (it.text == "LEVEL 2" && it.is_clicked == true)
        {
            this->inGame = true;
            graphics->flagTheStart();
            return true;
        }
    }
    return false;
}


bool Menu::actThreeInitiated(GraphicsHandler* graphics)
{
    for(auto it : this->buttons)
    {
        if (it.text == "LEVEL 3" && it.is_clicked == true)
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

void Menu::upgradeMenu()
{
    this->linkedGraphics->drawButtons();
}

void Menu::displayUpgrades()
{
    this->defaultAllButtons();

    auto it = this->buttons.begin();
    for (int i = 0; i < this->buttons.size(); ++i)
    {
        if (i < this->buttons.size() - 3)
            (*it).visible = false;
        else
            (*it).visible = true;
        
        it++;
    }
}