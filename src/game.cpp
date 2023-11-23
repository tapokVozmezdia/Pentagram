#include "game.hpp"

void animateMainCharacter(GraphicsHandler* graphics, llint mc_id)
{
    Animation anim;

    std::list<Texture2D*> txt;
    txt.push_back(graphics->getTexture("robot.png"));
    txt.push_back(graphics->getTexture("robot2.png"));
    std::list<std::string> nTxt;
    nTxt.push_back("robot.png");
    nTxt.push_back("robot2.png");
    std::vector<double> tQ;
    tQ.push_back(1.2);
    tQ.push_back(1.2);

    anim.textureNames = nTxt;
    anim.textures = txt;
    anim.timeQueues = tQ;

    graphics->animateTexture(mc_id, anim);

    for(int i = 1; i <= 5; ++i) 
        graphics->loadTextureFromImage("rA" + std::to_string(i) + ".png");
    
    Animation at;

    std::list<Texture2D*> txt1;
    std::list<std::string> nTxt1;
    std::vector<double> tQ1;
    for(int i = 1; i <= 5; ++i) 
    {
        txt1.push_back(graphics->getTexture("rA" + std::to_string(i) + ".png"));
        nTxt1.push_back("rA" + std::to_string(i) + ".png");
        tQ1.push_back(0.05);
    }

    at.textureNames = nTxt1;
    at.textures = txt1;
    at.timeQueues = tQ1;

    graphics->animateEntity(mc_id, at, at);
}







void Game::makeEnemy(const int x, const int y)
{
    llint enemy_id = (this->graphics.spawnEntity("dummy.png", x, y, 5, 100));
    this->objectNameMap["enemy" + std::to_string(enemy_id)] = enemy_id;

    Animation anim;

    std::list<Texture2D*> txt;
    std::list<std::string> nTxt;
    std::vector<double> tQ;

    txt.push_back(this->graphics.getTexture("dummy.png"));
    txt.push_back(this->graphics.getTexture("dummy2.png"));
    nTxt.push_back("dummy.png");
    nTxt.push_back("dummy2.png");
    tQ.push_back(0.8);
    tQ.push_back(0.8);

    anim.textureNames = nTxt;
    anim.textures = txt;
    anim.timeQueues = tQ;

    this->graphics.animateTexture(enemy_id, anim);

}

void Game::makeTrap(const int x, const int y)
{
    llint enemy_id = (this->graphics.spawnEntity("trap1.png", x, y, 50, 100));
    this->objectNameMap["trap" + std::to_string(enemy_id)] = enemy_id;
    
    this->graphics.trapFromEntity(enemy_id);

    Animation anim;

    std::list<Texture2D*> txt;
    std::list<std::string> nTxt;
    std::vector<double> tQ;

    for (int i = 0; i < 4; ++i)
    {
        txt.push_back(this->graphics.getTexture("trap1.png"));
        txt.push_back(this->graphics.getTexture("trap2.png"));
        nTxt.push_back("trap1.png");
        nTxt.push_back("trap2.png");
    }


    tQ.push_back(2);
    tQ.push_back(0.1);
    tQ.push_back(0.1);
    tQ.push_back(0.1);
    tQ.push_back(0.1);
    tQ.push_back(0.1);
    tQ.push_back(0.1);
    tQ.push_back(0.1);

    anim.textureNames = nTxt;
    anim.textures = txt;
    anim.timeQueues = tQ;

    this->graphics.animateTexture(enemy_id, anim);



    Animation at;

    std::list<Texture2D*> txt1;
    std::list<std::string> nTxt1;
    std::vector<double> tQ1;
    txt1.push_back(this->graphics.getTexture("trap_a1.png"));
    txt1.push_back(this->graphics.getTexture("trap_a2.png"));
    nTxt1.push_back("trap_a1.png");
    nTxt1.push_back("trap_a2.png");
    tQ1.push_back(0.1);
    tQ1.push_back(0.25);

    at.textureNames = nTxt1;
    at.textures = txt1;
    at.timeQueues = tQ1;

    this->graphics.animateEntity(enemy_id, at, at);

}




Game::Game(uint fps, uint screenWidth,  uint screenHeight)
{
    if (fps <= 0 || screenWidth <= 0 || screenHeight <= 0)
    {
        throw std::underflow_error("invalid arguments for constructor of Game class");
    }

    InitWindow(screenWidth, screenHeight, "Project Pentagram");
    InitAudioDevice();
    SetTargetFPS(fps);
}

Game::Game(uint screenWidth,  uint screenHeight)
{
    if (screenWidth <= 0 || screenHeight <= 0)
    {
        throw std::underflow_error("invalid arguments for constructor of Game class");
    }

    InitWindow(screenWidth, screenHeight, "Project Pentagram");
    InitAudioDevice();
    SetTargetFPS(60);
}

Game::~Game()
{
    CloseAudioDevice();
    CloseWindow();
}

void Game::run()
{
    this->actOne();

    while(!WindowShouldClose())
    {
        Controls::moveWASD(this->objectNameMap["MC"], &(this->graphics));
        Controls::attackLMC(this->objectNameMap["MC"], &(this->graphics));
        for (int i = 0; i < this->starNum; ++i)
        {
            Controls::vibrate(this->objectNameMap["star" + std::to_string(i)], 
                &(this->graphics), 1);
        }
        this->graphics.perform();
    }
}

void Game::actOne()
{
    this->graphics.loadTextureFromImage("robot.png");
    this->graphics.loadTextureFromImage("robot2.png");
    this->graphics.loadTextureFromImage("star.png");
    this->graphics.loadTextureFromImage("collision.png");
    this->graphics.loadTextureFromImage("attack.png");
    this->starNum = GetRandomValue(5, 30);
    for (int i = 0; i < this->starNum; ++i)
    {
        int x = GetRandomValue(-1000, 1000);
        int y = GetRandomValue(-1000, 1000);
        llint tmp = (this->graphics.spawnTexture("star.png", x, y));
        this->objectNameMap["star" + std::to_string(i)] = tmp;
    }


    // WARNING
    // THIS DRAINS MEMORY BCS RIGHT NOW TO EVERY ENEMY THERE IS A UNIQUE 
    // ANIMATION, BUT THEY ALL COULD BE THE SAME SO ONLY ONE TAKES MEMORY
    // MUST BE REWRITTEN WITH PROPER POINTERS

    for (int i = 0; i < 5; ++i)
    {
        this->makeEnemy(
            GetRandomValue(-1000, 1000),
            GetRandomValue(-1000, 1000)
        );
    }

    for (int i = 0; i < 5; ++i)
    {
        this->makeTrap(
            GetRandomValue(-1000, 1000),
            GetRandomValue(-1000, 1000)
        );
    }

    llint mc_id = (this->graphics.spawnEntity("robot.png", 100, 100, 10, 1000));
    this->objectNameMap["MC"] = mc_id;
    // std::cout << "mc_id: " << std::endl;
    // std::cout << mc_id << std::endl;
    this->graphics.centerCamera(mc_id);

    this->graphics.loadTextureFromImage("dummy.png");
    this->graphics.loadTextureFromImage("dummy2.png");

    this->graphics.loadTextureFromImage("trap1.png");
    this->graphics.loadTextureFromImage("trap2.png");
    this->graphics.loadTextureFromImage("trap_a1.png");
    this->graphics.loadTextureFromImage("trap_a2.png");

    animateMainCharacter(&(this->graphics), mc_id);
}
