#include "game.hpp"


Game::Game(uint fps, uint screenWidth,  uint screenHeight)
{
    if (fps <= 0 || screenWidth <= 0 || screenHeight <= 0)
    {
        throw std::underflow_error("invalid arguments for constructor of Game class");
    }

    this->menu.linkMenu(&(this->graphics));

    this->graphics.linkAudio(&(this->audio));

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

    this->menu.linkMenu(&(this->graphics));

    this->graphics.linkAudio(&(this->audio));

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
    this->loadTextures();
    this->loadSounds();


    this->audio.playAmbient("menumusic.mp3", 0.5);
    this->audio.setCombatMusic("track2.mp3", 0.5);


    while(!WindowShouldClose())
    {
        if (this->menu.actOneInitiated(&(this->graphics)))
        {
            // std::cout << "MOGUS MOGUS MOGUS" << std::endl;
            this->actOne();
        }

        if (this->menu.actTwoInitiated(&(this->graphics)))
        {
            // std::cout << "MOGUS MOGUS MOGUS" << std::endl;
            this->actTwo();
        }

        if (!(this->menu.isOn()) && !(this->menu.gameEnded()))
        {
            Controls::moveWASD(this->objectNameMap["MC"], &(this->graphics));
            Controls::attackLMC(this->objectNameMap["MC"], &(this->graphics));
            Controls::attackRMC(this->objectNameMap["MC"], &(this->graphics));

            for (int i = 0; i < this->starNum; ++i)
            {
                Controls::vibrate(this->objectNameMap["star" + std::to_string(i)], 
                    &(this->graphics), 1);

            }
        }

        if (this->menu.gameEnded() && this->menu.menuInGameFlag())
        {
            this->audio.stopAmbient();
            this->audio.playAmbient("menumusic.mp3", 0.5);
            this->menu.defaultAllButtons();
            this->objectNameMap.clear();
            starNum = 0;
        }

        this->graphics.perform();

        this->audio.perform();

    }
}





void Game::makeCharacter(const int x, const int y)
{
    llint mc_id = (this->graphics.spawnEntity("robot.png", x - 100, y - 100, 12, 400, Team::MAIN));
    this->objectNameMap["MC"] = mc_id;
    // std::cout << "mc_id: " << std::endl;
    // std::cout << mc_id << std::endl;
    this->graphics.centerCamera(mc_id);

    Animation anim;

    std::list<Texture2D*> txt;
    std::list<std::string> nTxt;
    std::vector<double> tQ;

    tQ.push_back(3);
    txt.push_back(this->graphics.getTexture("rob_r2.png"));
    nTxt.push_back("rob_r2.png");


    txt.push_back(this->graphics.getTexture("rob1.png"));
    nTxt.push_back("rob1.png");
    tQ.push_back(1);

    for (int j = 0; j < 100; ++j)
    {
        for(int i = 2; i <= 6; ++i)
        {
            txt.push_back(this->graphics.getTexture("rob" + std::to_string(i) + ".png"));
            nTxt.push_back("rob" + std::to_string(i) + ".png");
            tQ.push_back(0.05);
        }

        txt.push_back(this->graphics.getTexture("rob1.png"));
        nTxt.push_back("rob1.png");
        tQ.push_back(0.5);
    }

    anim.textureNames = nTxt;
    anim.textures = txt;
    anim.timeQueues = tQ;

    this->graphics.animateTexture(mc_id, anim);
    
    Animation at;

    std::list<Texture2D*> txt1;
    std::list<std::string> nTxt1;
    std::vector<double> tQ1;
    for(int i = 1; i <= 5; ++i) 
    {
        txt1.push_back(this->graphics.getTexture("rob_a" + std::to_string(i) + ".png"));
        nTxt1.push_back("rob_a" + std::to_string(i) + ".png");
    }

    tQ1.push_back(0.05);
    tQ1.push_back(0.03);
    tQ1.push_back(0.03);
    tQ1.push_back(0.03);
    tQ1.push_back(0.07);

    at.textureNames = nTxt1;
    at.textures = txt1;
    at.timeQueues = tQ1;

    at.soundNames.push_back("swordslice11.mp3");
    at.soundNames.push_back("swordslice12.mp3");
    at.soundNames.push_back("swordslice13.mp3");
    at.soundNames.push_back("swordslice14.mp3");

    at.extraSound.push_back("sword_hit2.mp3");

    this->graphics.animateEntity(mc_id, at, at);


    Animation run;

    std::list<Texture2D*> txt2;
    std::list<std::string> nTxt2;
    std::vector<double> tQ2;

    for(int i = 1; i <= 8; ++i) 
    {
        txt2.push_back(this->graphics.getTexture("rob_r" + std::to_string(i) + ".png"));
        nTxt2.push_back("rob_r" + std::to_string(i) + ".png");
        tQ2.push_back(0.05);
    }

    //txt2.push_back(this->graphics.getTexture("rob_r3.png"));
    //nTxt2.push_back("rob_r3.png");
    

    //double mod = 4;

    // tQ2.push_back(0.05 * mod);
    // //tQ2.push_back(0.05 * mod);
    // tQ2.push_back(0.05 * mod);
    // tQ2.push_back(0.04 * mod);
    // tQ2.push_back(0.05 * mod);
    //tQ2.push_back(0.08);
    //tQ2.push_back(0.05);

    run.textureNames = nTxt2;
    run.textures = txt2;
    run.timeQueues = tQ2;
    run.soundNames.push_back("rob_run.mp3");


    this->graphics.animateEntityMoving(mc_id, run);
    this->graphics.setHitbox(mc_id, {44, 96});
}





void Game::makeEnemy(int x, int y)
{
    x = x * 200;
    y = y * 200;

    llint enemy_id = (this->graphics.spawnEntity("n_enemy1.png", x, y, 15, 100, Team::HOSTILE));
    this->objectNameMap["enemy" + std::to_string(enemy_id)] = enemy_id;

    Animation anim;

    std::list<Texture2D*> txt;
    std::list<std::string> nTxt;
    std::vector<double> tQ;

    for (int i = 0; i < 2; i++)
    {
        txt.push_back(this->graphics.getTexture("n_enemy1.png"));
        txt.push_back(this->graphics.getTexture("n_enemy2.png"));
        nTxt.push_back("n_enemy1.png");
        nTxt.push_back("n_enemy2.png");
        tQ.push_back(0.8);
        tQ.push_back(0.5);
    }

    txt.push_back(this->graphics.getTexture("n_enemy2.png"));
    nTxt.push_back("n_enemy2.png");
    tQ.push_back(0.2);

    for (int i = 0; i <= 3; ++i)
    {
        txt.push_back(this->graphics.getTexture("n_enemy3.png"));
        txt.push_back(this->graphics.getTexture("n_enemy2.png"));
        nTxt.push_back("n_enemy3.png");
        nTxt.push_back("n_enemy2.png");
        tQ.push_back(0.2);
        tQ.push_back(0.2);
    }


    anim.textureNames = nTxt;
    anim.textures = txt;
    anim.timeQueues = tQ;

    this->graphics.animateTexture(enemy_id, anim);

    Animation at;

    std::list<Texture2D*> txt1;
    std::list<std::string> nTxt1;
    std::vector<double> tQ1;

    for (int i = 0; i < 10; ++i)
    {
        txt1.push_back(this->graphics.getTexture("n_enemy_zap1.png"));
        txt1.push_back(this->graphics.getTexture("n_enemy_zap2.png"));
        nTxt1.push_back("n_enemy_zap1.png");
        nTxt1.push_back("n_enemy_zap1.png");
        tQ1.push_back(0.05);
        tQ1.push_back(0.05);
    }

    at.textureNames = nTxt1;
    at.textures = txt1;
    at.timeQueues = tQ1;

    at.soundNames.push_back("zap.mp3");

    this->graphics.animateEntity(enemy_id, at, at);

    this->graphics.setHitbox(enemy_id,{38,60});

}

void Game::spawnCrawler(int x, int y)
{
    x = x * 200;
    y = y * 200;

    llint enemy_id = (this->graphics.spawnEntity("crawler1.png", x, y, 100, 500, Team::HOSTILE));
    this->objectNameMap["enemy" + std::to_string(enemy_id)] = enemy_id;


    Animation anim;

    anim.textures.push_back(this->graphics.getTexture("crawler1.png"));
    anim.textureNames.push_back("crawler1.png");
    anim.timeQueues.push_back(4);

    for (int i = 1; i <= 6; ++i)
    {
        anim.textures.push_back(this->graphics.getTexture("crawler" + 
        std::to_string(i) + ".png"));
        anim.textureNames.push_back("crawler" + 
        std::to_string(i) + ".png");
        anim.timeQueues.push_back(0.08);
    }
    for (int i = 0; i < 10; ++i)
    {
        anim.textures.push_back(this->graphics.getTexture("crawler5.png"));
        anim.textures.push_back(this->graphics.getTexture("crawler6.png"));
        anim.textureNames.push_back("crawler5.png");
        anim.textureNames.push_back("crawler6.png");
        anim.timeQueues.push_back(0.03);
        anim.timeQueues.push_back(0.03);
    }
    for (int i = 6; i >= 1; i--)
    {
        anim.textures.push_back(this->graphics.getTexture("crawler" + 
        std::to_string(i) + ".png"));
        anim.textureNames.push_back("crawler" + 
        std::to_string(i) + ".png");
        anim.timeQueues.push_back(0.08);
    }


    this->graphics.animateTexture(enemy_id, anim);


    Animation at;

    std::list<Texture2D*> txt;
    std::list<std::string> nTxt;
    std::vector<double> tQ;

    txt.push_back(this->graphics.getTexture("crawler1.png"));
    nTxt.push_back("crawler1.png");
    tQ.push_back(1);

    at.textureNames = nTxt;
    at.textures = txt;
    at.timeQueues = tQ;

    this->graphics.animateEntity(enemy_id, at, at);

    Animation run;

    for(int i = 1; i <= 3; ++i)
    {
        run.textures.push_back(this->graphics.getTexture(
            "crawler_walk" + std::to_string(i) + ".png"
        ));
        run.textureNames.push_back("crawler_walk" + std::to_string(i) + ".png");
        run.timeQueues.push_back(0.05);
    }

    run.textures.push_back(this->graphics.getTexture(
        "crawler1.png"
    ));
    run.textureNames.push_back("crawler1.png");
    run.timeQueues.push_back(0.05);

    for(int i = 4; i <= 6; ++i)
    {
        run.textures.push_back(this->graphics.getTexture(
            "crawler_walk" + std::to_string(i) + ".png"
        ));
        run.textureNames.push_back("crawler_walk" + std::to_string(i) + ".png");
        run.timeQueues.push_back(0.05);
    }

    run.textures.push_back(this->graphics.getTexture(
        "crawler1.png"
    ));
    run.textureNames.push_back("crawler1.png");
    run.timeQueues.push_back(0.05);

    this->graphics.animateEntityMoving(enemy_id, run);

    this->graphics.setHitbox(enemy_id, {125, 125});
}

void Game::makeTrap(int x, int y)
{
    x = x * 200;
    y = y * 200;

    llint enemy_id = (this->graphics.spawnEntity("trap1.png", x, y + 75, 10, 100, Team::HOSTILE));
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
    for (int i = 0; i < 7; ++i) tQ.push_back(0.1);

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
    txt1.push_back(this->graphics.getTexture("trap_a1.png"));
    txt1.push_back(this->graphics.getTexture("trap_a2.png"));
    nTxt1.push_back("trap_a1.png");
    nTxt1.push_back("trap_a2.png");
    nTxt1.push_back("trap_a1.png");
    nTxt1.push_back("trap_a2.png");
    tQ1.push_back(0.05);
    tQ1.push_back(0.03);
    tQ1.push_back(0.05);
    tQ1.push_back(0.04);

    at.textureNames = nTxt1;
    at.textures = txt1;
    at.timeQueues = tQ1;

    this->graphics.animateEntity(enemy_id, at, at);

}

void Game::makeHeal(int x, int y)
{
    x = x * 200;
    y = y * 200;

    llint heal_id = (this->graphics.spawnEntity("heal1.png", x + 75, y + 75, -50, 1, Team::HOSTILE));
    this->objectNameMap["heal" + std::to_string(heal_id)] = heal_id;
    
    this->graphics.trapFromEntity(heal_id);

    Animation anim;

    std::list<Texture2D*> txt;
    std::list<std::string> nTxt;
    std::vector<double> tQ;

    for (int i = 1; i <= 5; ++i)
    {
        txt.push_back(this->graphics.getTexture("heal" + std::to_string(i) + ".png"));
        nTxt.push_back("heal" + std::to_string(i) + ".png");
        tQ.push_back(0.1);
    }

    for (int i = 5; i >= 1; i--)
    {
        txt.push_back(this->graphics.getTexture("heal" + std::to_string(i) + ".png"));
        nTxt.push_back("heal" + std::to_string(i) + ".png");
        tQ.push_back(0.1);
    }

    anim.textureNames = nTxt;
    anim.textures = txt;
    anim.timeQueues = tQ;

    this->graphics.animateTexture(heal_id, anim);

    this->graphics.animateEntity(heal_id, anim, anim);

}

void Game::makeWall(const int x, const int y)
{
    llint wall_id = (this->graphics.spawnTexture("nwall.png", x, y));
    this->objectNameMap["wall" + std::to_string(wall_id)] = wall_id;

    this->graphics.wallFromObject(wall_id);
}


void Game::makeFloor(const int x, const int y)
{
    //int rand = GetRandomValue(1, 4);
    // if (rand != 4)
    // {
    //     llint floor_id = (this->graphics.spawnTexture(("floor" + std::to_string(rand) + ".png"), x, y));
    //     this->objectNameMap["floor" + std::to_string(floor_id)] = floor_id;
    //     return;
    // }

    int rand = GetRandomValue(1, 11);
    llint floor_id;
    if (rand != 3)
        floor_id = (this->graphics.spawnTexture(("pfloor.png"), x, y));
    else
        floor_id = (this->graphics.spawnTexture(("nnfloor.png"), x, y));
    this->objectNameMap["floor" + std::to_string(floor_id)] = floor_id;

    // Animation anim;

    // std::list<Texture2D*> txt;
    // std::list<std::string> nTxt;
    // std::vector<double> tQ;

    // // for (int i = 1; i <= 4; ++i)
    // // {
    // //     txt.push_back(this->graphics.getTexture("floor4" + std::to_string(i) + ".png"));
    // //     nTxt.push_back("floor4" + std::to_string(i) + ".png");
    // // }

    // txt.push_back(this->graphics.getTexture("floor41.png"));
    // nTxt.push_back("floor41.png");

    // for (int i = 0; i < 3; ++i)
    // {
    //     txt.push_back(this->graphics.getTexture("floor42.png"));
    //     nTxt.push_back("floor42.png");
    //     txt.push_back(this->graphics.getTexture("floor41.png"));
    //     nTxt.push_back("floor41.png");
    // }

    // txt.push_back(this->graphics.getTexture("floor43.png"));
    // nTxt.push_back("floor43.png");

    // txt.push_back(this->graphics.getTexture("floor44.png"));
    // nTxt.push_back("floor44.png");

    // tQ.push_back(1.8);
    // for (int i = 0; i < 6; ++i) 
    //     tQ.push_back(0.1);
    // tQ.push_back(0.2);
    // tQ.push_back(1.4);

    // anim.textureNames = nTxt;
    // anim.textures = txt;
    // anim.timeQueues = tQ;

    // this->graphics.animateTexture(floor_id, anim);
}



void Game::launchEnergyBall(int x, int y, int t_x, int t_y)
{
    Animation a1, a2;

    std::list<Texture2D*> txt;
    std::list<std::string> nTxt;
    std::vector<double> tQ;

    std::list<Texture2D*> txt1;
    std::list<std::string> nTxt1;
    std::vector<double> tQ1;

    for (int i = 1; i <= 5; ++i)
    {
        txt.push_back(this->graphics.getTexture("energy_ball" + std::to_string(i) + ".png"));
        nTxt.push_back("energy_ball" + std::to_string(i) + ".png");
        tQ.push_back(0.07);
    }

    for (int i = 1; i <= 9; ++i)
    {
        txt1.push_back(this->graphics.getTexture("eb_exp" + std::to_string(i) + ".png"));
        nTxt1.push_back("eb_exp" + std::to_string(i) + ".png");
        if (i != 9)
            tQ1.push_back(0.09);
    }
    tQ1.push_back(2);

    a1.textures = txt;
    a1.textureNames = nTxt;
    a1.timeQueues = tQ;

    a2.textures = txt1;
    a2.textureNames = nTxt1;
    a2.timeQueues = tQ1;

    this->graphics.fireProjectile(
        "energy_ball1.png", {(float)x, (float)y}, {(float)t_x, (float)t_x}, 
        60, 1, a1, a2
    );
}



void Game::createRoom(int x, int y, const int wdth, const int hgth, const Directions& exits)
{
    x = x * 200;
    y = y * 200;

    if (wdth == 0 || hgth == 0)
    {
        throw std::logic_error("Trying to create a zero-size room");
    }

    for (int i = 0; i < hgth; ++i)
    {
        for (int j = 0; j < wdth; ++j)
        {
            this->makeFloor(x + (j * 200), y + (i * 200));
        
            if (i == 0)
            {
                if (!(exits.top && (
                    (wdth % 2 == 1 && j == wdth / 2) 
                    || (wdth % 2 == 0 && (j == wdth / 2 || j == wdth / 2 - 1))
                    )))
                    this->makeWall(x + (j * 200), y - 200);
                else
                    this->makeFloor(x + (j * 200), y - 200);
                if (!(exits.down && (
                    (wdth % 2 == 1 && j == wdth / 2) 
                    || (wdth % 2 == 0 && (j == wdth / 2 || j == wdth / 2 - 1))
                    )))
                    this->makeWall(x + (j * 200), y + (hgth * 200));
                else
                    this->makeFloor(x + (j * 200), y + (hgth * 200));
            }
        }
        if (!(exits.left && (
            (hgth % 2 == 1 && i == hgth / 2) 
            || (hgth % 2 == 0 && (i == hgth / 2 || i == hgth / 2 - 1))
            )))
            this->makeWall(x - 200, y + (i * 200));
        else
            this->makeFloor(x - 200, y + (i * 200));
        if (!(exits.right && (
            (hgth % 2 == 1 && i == hgth / 2) 
            || (hgth % 2 == 0 && (i == hgth / 2 || i == hgth / 2 - 1))
            )))
            this->makeWall(x + (wdth * 200), y + (i * 200));
        else
            this->makeFloor(x + (wdth * 200), y + (i * 200));
    }
    this->makeWall(x - 200, y - 200);
    this->makeWall(x - 200, y + (hgth * 200));
    this->makeWall(x + (wdth * 200), y - 200);
    this->makeWall(x + (wdth * 200), y + (hgth * 200));
}


void Game::createWallBlock(int x, int y, const int wdth, const int hgth)
{
    x = x * 200;
    y = y * 200;

    if (wdth == 0 || hgth == 0)
    {
        this->makeWall(x, y);
        return;
    }

    for (int i = 0; i < hgth; ++i)
    {
        for (int j = 0; j < wdth; ++j)
        {
            this->makeWall(x + (j * 200), y + (i * 200));
        }
    }
}

void Game::createFloorBlock(int x, int y, const int wdth, const int hgth)
{
    x = x * 200;
    y = y * 200;

    if (wdth == 0 || hgth == 0)
    {
        this->makeFloor(x, y);
        return;
    }

    for (int i = 0; i < hgth; ++i)
    {
        for (int j = 0; j < wdth; ++j)
        {
            this->makeFloor(x + (j * 200), y + (i * 200));
        }
    }
}

void Game::createHallway(int x, int y, const SingularDirection dir, const uint wdth, const uint length)
{
    x = x * 200;
    y = y * 200;

    if (dir == LEFT || dir == RIGHT)
    {
        for (int i = 0; i < length; ++i)
        {
            this->makeWall(i * 200 * (dir - 1) + x - abs(2 - dir) * 100, y - 200);
            for (int j = 0; j < wdth; ++j)
            {
                this->makeFloor(i * 200 * (dir - 1) + x - abs(2 - dir) * 100, y + j * (200));
            }
            this->makeWall(i * 200 * (dir - 1) + x - abs(2 - dir) * 100, y + 200 * wdth);
        }
        return;
    }
    for (int i = 0; i < length; ++i)
    {
        this->makeWall(x - 200, y + (1 - dir) * 100 + i * (2 - dir) * 200);
        for (int j = 0; j < wdth; ++j)
        {
            this->makeFloor(x + j * 200, y + (1 - dir) * 100 + i * (2 - dir) * 200);
        }
        this->makeWall(x + 200 * wdth, y + (1 - dir) * 100 + i * (2 - dir) * 200);
    }
}

void Game::loadTextures()
{
    this->graphics.loadTextureFromImage("robot.png");
    this->graphics.loadTextureFromImage("robot2.png");
    this->graphics.loadTextureFromImage("star.png");
    this->graphics.loadTextureFromImage("collision.png");
    this->graphics.loadTextureFromImage("attack.png");

    this->graphics.loadTextureFromImage("dummy.png");
    this->graphics.loadTextureFromImage("dummy2.png");

    this->graphics.loadTextureFromImage("dummy_strike.png");

    this->graphics.loadTextureFromImage("n_enemy1.png");
    this->graphics.loadTextureFromImage("n_enemy2.png");
    this->graphics.loadTextureFromImage("n_enemy3.png");
    this->graphics.loadTextureFromImage("n_enemy_zap1.png");
    this->graphics.loadTextureFromImage("n_enemy_zap2.png");

    this->graphics.loadTextureFromImage("trap1.png");
    this->graphics.loadTextureFromImage("trap2.png");
    this->graphics.loadTextureFromImage("trap_a1.png");
    this->graphics.loadTextureFromImage("trap_a2.png");

    this->graphics.loadTextureFromImage("nwall.png");


    this->graphics.loadTextureFromImage("nfloor.png");
    this->graphics.loadTextureFromImage("nfloor1.png");
    this->graphics.loadTextureFromImage("nnfloor.png");
    this->graphics.loadTextureFromImage("nnfloor1.png");
    this->graphics.loadTextureFromImage("pfloor.png");
    // this->graphics.loadTextureFromImage("floor1.png");
    // this->graphics.loadTextureFromImage("floor2.png");
    // this->graphics.loadTextureFromImage("floor3.png");
    // this->graphics.loadTextureFromImage("floor41.png");
    // this->graphics.loadTextureFromImage("floor42.png");
    // this->graphics.loadTextureFromImage("floor43.png");
    // this->graphics.loadTextureFromImage("floor44.png");

    this->graphics.loadTextureFromImage("roboss1.png");

    for (int i = 1; i <= 9; ++i)
        this->graphics.loadTextureFromImage("eb_exp" + std::to_string(i) + ".png");

    for (int i = 1; i <= 5; ++i) 
    {
        this->graphics.loadTextureFromImage("heal" + std::to_string(i) + ".png");
        this->graphics.loadTextureFromImage("energy_ball" + std::to_string(i) + ".png");
    }

    for(int i = 1; i <= 5; ++i) 
    {
        this->graphics.loadTextureFromImage("rob_a" + std::to_string(i) + ".png");
        this->graphics.loadTextureFromImage("rob" + std::to_string(i) + ".png");
        this->graphics.loadTextureFromImage("rob_r" + std::to_string(i) + ".png");
        this->graphics.loadTextureFromImage("crawler" + std::to_string(i) + ".png");
        this->graphics.loadTextureFromImage("crawler_walk" + std::to_string(i) + ".png");
    }
    this->graphics.loadTextureFromImage("crawler_walk6.png");
    this->graphics.loadTextureFromImage("crawler6.png");
    this->graphics.loadTextureFromImage("rob6.png");
    this->graphics.loadTextureFromImage("rob_r6.png");
    this->graphics.loadTextureFromImage("rob_r7.png");
    this->graphics.loadTextureFromImage("rob_r8.png");
}


void Game::loadSounds()
{
    this->audio.loadSoundByName("ost1.mp3");
    this->audio.loadSoundByName("swordslice11.mp3");
    this->audio.loadSoundByName("swordslice12.mp3");
    this->audio.loadSoundByName("swordslice13.mp3");
    this->audio.loadSoundByName("swordslice14.mp3");

    this->audio.loadSoundByName("rob_run.mp3");

    this->audio.loadSoundByName("sword_hit2.mp3");

    this->audio.loadSoundByName("menumusic.mp3");

    this->audio.loadSoundByName("track2.mp3");

    this->audio.loadSoundByName("zap.mp3");
}


void Game::actOne()
{
    // int c1 = -1000, c2 = -1000;

    // for (int i = 0; i < 10; ++i)
    // {
    //     for (int j = 0; j < 10; ++j)
    //     {
    //         this->makeFloor(c1 + (i * 200), c2 + (j * 200));
    //     }
    // }


    // WARNING
    // THIS DRAINS MEMORY BCS RIGHT NOW TO EVERY ENEMY THERE IS A UNIQUE 
    // ANIMATION, BUT THEY ALL COULD BE THE SAME SO ONLY ONE TAKES MEMORY
    // MUST BE REWRITTEN WITH PROPER POINTERS

    // for (int i = 0; i < 5; ++i)
    // {
    //     this->makeTrap(
    //         GetRandomValue(0, 1000),
    //         GetRandomValue(0, 1000)
    //     );
    // }

    // this->starNum = GetRandomValue(5, 10);
    // for (int i = 0; i < this->starNum; ++i)
    // {
    //     int x = GetRandomValue(0, 1000);
    //     int y = GetRandomValue(0, 1000);
    //     llint tmp = (this->graphics.spawnTexture("star.png", x, y));
    //     this->objectNameMap["star" + std::to_string(i)] = tmp;
    // }

    // llint mc_id = (this->graphics.spawnEntity("robot.png", 100, 100, 10, 1000, Team::MAIN));
    // this->objectNameMap["MC"] = mc_id;
    // // std::cout << "mc_id: " << std::endl;
    // // std::cout << mc_id << std::endl;
    // this->graphics.centerCamera(mc_id);

    this->createRoom(-2, -2, 4, 4, Directions{0, 0, 1, 0}); // 1st room
    this->createHallway(3, -1, RIGHT, 2, 5);
    this->createRoom(8, -4, 8, 8, Directions{1, 1, 0, 1}); // 2nd room
    this->createWallBlock(10, -2, 4, 4);
    this->createHallway(11, 5, DOWN, 2, 2);
    this->createRoom(10, 7, 4, 2, Directions{0, 0, 0, 1}); // 3rd room
    this->createWallBlock(10, -8, 1, 3);
    this->createFloorBlock(11, -7, 14, 2);
    this->createWallBlock(11, -8, 14, 1);
    this->createWallBlock(17, -5, 6, 1);
    this->createWallBlock(25, -8, 1, 3);
    this->createRoom(20, -4, 8, 8, Directions{0, 1, 0, 1}); // 4th room

    this->createWallBlock(23, -1, 2, 2);

    this->createWallBlock(20, -2, 1, 1);
    this->createWallBlock(20, 1, 1, 1);
    this->createWallBlock(27, -2, 1, 1);
    this->createWallBlock(27, 1, 1, 1);

    this->createHallway(23, 5, DOWN, 2, 2);
    this->createRoom(22, 7, 4, 4, Directions{0, 0, 1, 1}); // 5th room

    this->createHallway(27, 8, RIGHT, 2, 3);
    this->createWallBlock(30, 10, 3, 1);
    this->createWallBlock(32, 5, 1, 5);
    this->createFloorBlock(30, 3, 5, 2);
    this->createFloorBlock(30, 5, 2, 5);
    this->createWallBlock(29, 2, 1, 5);
    this->createWallBlock(30, 2, 6, 1);
    this->createWallBlock(35, 3, 1, 5);
    this->createFloorBlock(33, 5, 2, 5);
    this->createWallBlock(35, 3, 1, 2);
    this->createWallBlock(33, 10, 3, 1);

    this->createRoom(36, 6, 10, 6, Directions{1, 0, 1, 0}); // 6th room
    this->createWallBlock(38, 8, 5, 2);

    this->createHallway(47, 8, RIGHT, 2, 4);

    this->createRoom(51, 5, 6, 8, Directions{1, 0, 0, 0}); // Boss room


    this->makeTrap(8, -4);
    this->makeTrap(8, -3);
    this->makeTrap(9, -4);
    this->makeTrap(9, -3);

    this->makeTrap(20, -1);
    this->makeTrap(20, 0);
    this->makeTrap(27, -1);
    this->makeTrap(27, 0);

    this->makeTrap(20, -4);
    this->makeTrap(27, -4);
    this->makeTrap(27, 3);
    this->makeTrap(20, 3);

    this->makeTrap(30, 7);
    this->makeTrap(31, 5);
    this->makeTrap(30, 3);
    this->makeTrap(32, 4);
    this->makeTrap(33, 5);
    this->makeTrap(34, 7);

    this->makeTrap(45, 6);
    this->makeTrap(45, 11);

    this->makeHeal(11, 8);
    this->makeHeal(12, 8);

    this->makeHeal(22, 10);
    this->makeHeal(23, 10);
    this->makeHeal(24, 10);
    this->makeHeal(25, 10);

    this->makeHeal(37, 6);
    this->makeHeal(37, 11);

    this->makeHeal(47, 8);
    this->makeHeal(47, 9);
    this->makeHeal(48, 8);
    this->makeHeal(48, 9);
    this->makeHeal(49, 8);
    this->makeHeal(49, 9);

    this->spawnCrawler(54, 9);

    //Traps & heals before, enemies after
    this->makeCharacter(0, 0);

    this->makeEnemy(4, 0);
    this->makeEnemy(12, 2);
    this->makeEnemy(11, 6);
    this->makeEnemy(12, 6);

    this->makeEnemy(15, -7);
    this->makeEnemy(15, -6);

    this->makeEnemy(20, -7);
    this->makeEnemy(20, -6);

    this->makeEnemy(23, -3);
    this->makeEnemy(24, -3);
    this->makeEnemy(23, -3);
    this->makeEnemy(24, -3);
    this->makeEnemy(24, -3);
    this->makeEnemy(23, -3);

    this->makeEnemy(23, 1);
    this->makeEnemy(24, 1);
    this->makeEnemy(23, 1);
    this->makeEnemy(24, 1);
    this->makeEnemy(24, 1);
    this->makeEnemy(23, 1);

    this->makeEnemy(24, 7);
    this->makeEnemy(24, 7);

    this->makeEnemy(28, 8);
    this->makeEnemy(28, 9);

    this->makeEnemy(42, 11);
    this->makeEnemy(42, 7);

    this->makeEnemy(44, 8);
    this->makeEnemy(44, 8);

    this->makeEnemy(44, 8);
    this->makeEnemy(44, 9);
    this->makeEnemy(44, 9);


    //this->audio.playOst("sclice.mp3");

    this->audio.stopAmbient();

    this->audio.playAmbient("ost1.mp3", 0.05);

    // this->graphics.fireProjectile();

    // for (int i = 0; i < 5; ++i)
    // {
    //     this->makeEnemy(
    //         GetRandomValue(0, 1000),
    //         GetRandomValue(0, 1000)
    //     );
    // }

}

void Game::actTwo()
{


    this->createRoom(-1, -1, 6, 3, {0, 0, 1, 0}); // 1st room

    this->createRoom(6, -2, 4, 5, {1, 1, 1, 1}); // 2nd room

    this->createHallway(7, -3, TOP, 2, 4);

    this->createWallBlock(6, -10, 1, 3);
    this->createWallBlock(7, -10, 3, 1);
    this->createFloorBlock(7, -9, 2, 2);

    this->createHallway(9, -9, RIGHT, 2, 4);
    this->createRoom(13, -9, 2, 2, {1, 1, 0, 0});
    this->createRoom(13, -6, 2, 2, {0, 0, 1, 1});

    this->createRoom(16, -8, 5, 6, {1, 0, 1, 1}); // 3rd room
    this->createRoom(22, -6, 2, 2, {1, 1, 1, 0});





    //Traps & heals before, enemies after
    this->makeCharacter(0, 0);





    this->audio.stopAmbient();

    this->audio.playAmbient("ost1.mp3", 0.05);
}