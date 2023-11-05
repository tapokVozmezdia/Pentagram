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
        Controls::attackLMC(this->objectNameMap["MC"], &(this->graphics));
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
    int starNum = GetRandomValue(5, 20);
    for (int i = 0; i < starNum; ++i)
    {
        int x = GetRandomValue(-1000, 1000);
        int y = GetRandomValue(-1000, 1000);
        llint tmp = (this->graphics.spawnTexture("star.png", x, y));
        this->objectNameMap["star" + std::to_string(tmp)] = tmp;
    }
    llint mc_id = (this->graphics.spawnEntity("robot.png", 100, 100, 10, 1000));
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


    this->graphics.loadTextureFromImage("rA1.png");
    this->graphics.loadTextureFromImage("rA2.png");
    this->graphics.loadTextureFromImage("rA3.png");
    this->graphics.loadTextureFromImage("rA4.png");
    this->graphics.loadTextureFromImage("rA5.png");
    
    Animation at;

    std::list<Texture2D*> txt1;
    txt1.push_back(this->graphics.getTexture("rA1.png"));
    txt1.push_back(this->graphics.getTexture("rA2.png"));
    txt1.push_back(this->graphics.getTexture("rA3.png"));
    txt1.push_back(this->graphics.getTexture("rA4.png"));
    txt1.push_back(this->graphics.getTexture("rA5.png"));
    std::list<std::string> nTxt1;
    nTxt1.push_back("rA1.png");
    nTxt1.push_back("rA2.png");
    nTxt1.push_back("rA3.png");
    nTxt1.push_back("rA4.png");
    nTxt1.push_back("rA5.png");
    std::vector<double> tQ1;
    tQ1.push_back(0.05);
    tQ1.push_back(0.05);
    tQ1.push_back(0.05);
    tQ1.push_back(0.05);
    tQ1.push_back(0.05);

    at.textureNames = nTxt1;
    at.textures = txt1;
    at.timeQueues = tQ1;

    this->graphics.animateEntity(mc_id, at, at);

}
