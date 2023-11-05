#include "game.hpp"

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
        this->graphics.perform();
    }
}

void Game::actOne()
{
    this->graphics.loadTextureFromImage("robot.png");
    this->graphics.loadTextureFromImage("robot2.png");
    this->graphics.loadTextureFromImage("star.png");
    this->graphics.loadTextureFromImage("collision.png");
    int starNum = GetRandomValue(5, 20);
    for (int i = 0; i < starNum; ++i)
    {
        int x = GetRandomValue(-1000, 1000);
        int y = GetRandomValue(-1000, 1000);
        llint tmp = (this->graphics.spawnTexture("star.png", x, y));
        this->objectNameMap["star" + std::to_string(tmp)] = tmp;
    }
    llint mc_id = (this->graphics.spawnTexture("robot.png", 100, 100));
    this->objectNameMap["MC"] = mc_id;
    // std::cout << "mc_id: " << std::endl;
    // std::cout << mc_id << std::endl;
    this->graphics.centerCamera(mc_id);

    
    Animation anim;

    std::list<Texture2D*> txt;
    txt.push_back(this->graphics.getTexture("robot.png"));
    txt.push_back(this->graphics.getTexture("robot2.png"));
    std::list<std::string> nTxt;
    nTxt.push_back("robot.png");
    nTxt.push_back("robot2.png");
    std::vector<double> tQ;
    tQ.push_back(1.2);
    tQ.push_back(1.2);

    anim.textureNames = nTxt;
    anim.textures = txt;
    anim.timeQueues = tQ;

    this->graphics.animateTexture(mc_id, anim);

}
