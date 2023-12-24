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
    for (int i = 0; i < 4; ++i)
        this->levelsPassed.push_back(false);


    this->loadTextures();
    this->loadSounds();

    
    this->graphics.linkProgressTracker(&(this->pTrack), &(this->levelsPassed), 
        &(this->difficulty), &(this->records));


    this->audio.playAmbient("menumusic.mp3", 0.5);
    this->audio.setCombatMusic("track2.mp3", 0.5);

    // this->graphics.enemyHostilityTriggerOff(); // FOR LEVEL-DESIGN PURPOSES

    //this->menu.displayUpgrades();


    while(!WindowShouldClose())
    {
        if (this->menu.actOneInitiated(&(this->graphics)))
        {
            // std::cout << "MOGUS MOGUS MOGUS" << std::endl;

            this->levelLaunched = 1;

            this->graphics.setCurLvl(1);

            this->actOne();
            this->graphics.syncAudio();
            this->records.startRecording();
        }

        if (this->menu.actTwoInitiated(&(this->graphics)))
        {
            // std::cout << "MOGUS MOGUS MOGUS" << std::endl;



            this->levelLaunched = 2;
            this->graphics.setCurLvl(2);

            this->actTwo();
            this->graphics.syncAudio();
            this->records.startRecording();
        }

        if (this->menu.actThreeInitiated(&(this->graphics)))
        {
            // std::cout << "MOGUS MOGUS MOGUS" << std::endl;



            this->levelLaunched = 3;
            this->graphics.setCurLvl(3);

            this->actThree();
            this->graphics.syncAudio();
            this->records.startRecording();
        }

        if (!(this->menu.isOn()) && !(this->menu.gameEnded()))
        {
            Controls::moveWASD(this->objectNameMap["MC"], &(this->graphics));
            Controls::attackLMC(this->objectNameMap["MC"], &(this->graphics));
            // Controls::attackRMC(this->objectNameMap["MC"], &(this->graphics));

            for (int i = 0; i < this->starNum; ++i)
            {
                Controls::vibrate(this->objectNameMap["star" + std::to_string(i)], 
                    &(this->graphics), 1);

            }
        }

        if (this->menu.gameEnded() && this->menu.menuInGameFlag()
            && !this->graphics.checkVictory())
        {
            this->audio.stopAmbient();
            this->audio.playAmbient("menumusic.mp3", 0.5);
            this->menu.defaultAllButtons();
            this->objectNameMap.clear();
            starNum = 0;
        }

        if (this->graphics.checkVictory() && this->menu.menuInGameFlag())
        {
            this->audio.stopAmbient();
            this->audio.playAmbient("menumusic.mp3", 0.5);
            if (!(this->levelsPassed[this->levelLaunched]))
                this->menu.displayUpgrades();
            else
                this->menu.defaultAllButtons();
            this->objectNameMap.clear();
            starNum = 0;
        }

        this->graphics.perform();

        this->audio.perform();

        // std::cout << this->difficulty << std::endl;

    }
}





void Game::makeCharacter(const int x, const int y)
{
    // def dmg = 12
    llint mc_id = (this->graphics.spawnEntity("robot.png", x, y, 
        12 + this->pTrack.AT_BUFFS * AT_BUFF, 
        400 + this->pTrack.HP_BUFFS * HP_BUFF, Team::MAIN));
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
    this->graphics.setBaseShield(mc_id, 200 + this->pTrack.SH_BUFFS * SH_BUFF);
}


void Game::makeCustomEnemy(int x, int y, int dmg, int hp)
{
    x = x * 200;
    y = y * 200;

    llint enemy_id = (this->graphics.spawnEntity("n_enemy1.png", x, y, 
        (int)(float(dmg) * (1 + ((float)this->difficulty)*0.5)), 
        (int)(float(hp) * (1 + ((float)this->difficulty)*0.25)), 
        Team::HOSTILE));
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


void Game::makeEnemy(int x, int y)
{
    this->makeCustomEnemy(x, y, 15, 100);
}

void Game::spawnEnhancedEnemy(int x, int y)
{
    this->makeCustomEnemy(x, y, 35, 180);
    this->graphics.setSpecialTint(this->graphics.getLastId(), RED_TINT);
}

void Game::makeCrawler(int x, int y, int dmg, int hp, float attackTime, bool big)
{
    // default attack time = 1.f
    // lower time = faster attacks
    x = x * 200;
    y = y * 200;

    std::string up = "";
    if (big)
        up = "SCALED_";

    llint enemy_id = (this->graphics.spawnEntity(up + "crawler1.png", x, y, 
    (int)((float)dmg * (1 + ((float)this->difficulty)*0.5)), 
    (int)((float)hp * (1 + ((float)this->difficulty)*0.25)), 
    Team::HOSTILE));
    this->objectNameMap["enemy" + std::to_string(enemy_id)] = enemy_id;


    Animation anim;

    anim.textures.push_back(this->graphics.getTexture(up + "crawler1.png"));
    anim.textureNames.push_back(up + "crawler1.png");
    anim.timeQueues.push_back(4);

    for (int i = 1; i <= 6; ++i)
    {
        anim.textures.push_back(this->graphics.getTexture(up + "crawler" + 
        std::to_string(i) + ".png"));
        anim.textureNames.push_back(up + "crawler" + 
        std::to_string(i) + ".png");
        anim.timeQueues.push_back(0.08);
    }
    for (int i = 0; i < 10; ++i)
    {
        anim.textures.push_back(this->graphics.getTexture(up + "crawler5.png"));
        anim.textures.push_back(this->graphics.getTexture(up + "crawler6.png"));
        anim.textureNames.push_back(up + "crawler5.png");
        anim.textureNames.push_back(up + "crawler6.png");
        anim.timeQueues.push_back(0.03);
        anim.timeQueues.push_back(0.03);
    }
    for (int i = 6; i >= 1; i--)
    {
        anim.textures.push_back(this->graphics.getTexture(up + "crawler" + 
        std::to_string(i) + ".png"));
        anim.textureNames.push_back(up + "crawler" + 
        std::to_string(i) + ".png");
        anim.timeQueues.push_back(0.08);
    }


    this->graphics.animateTexture(enemy_id, anim);


    Animation at;

    for (int i = 1; i <= 4; ++i)
    {
        at.textures.push_back(this->graphics.getTexture(up + "crawler_a" + 
        std::to_string(i) + ".png"));
        at.textureNames.push_back(up + "crawler_a" + 
        std::to_string(i) + ".png");
    }

    at.textures.push_back(this->graphics.getTexture(up + "crawler1.png"));
    at.textureNames.push_back(up + "crawler1.png");

    at.timeQueues.push_back(0.5 * attackTime);
    at.timeQueues.push_back(0.08 * attackTime);
    at.timeQueues.push_back(0.04 * attackTime);
    at.timeQueues.push_back(0.15 * attackTime);
    at.timeQueues.push_back(1 * attackTime);

    at.soundNames.push_back("fazedstomp.mp3");

    this->graphics.animateEntity(enemy_id, at, at);

    Animation run;

    for(int i = 1; i <= 3; ++i)
    {
        run.textures.push_back(this->graphics.getTexture(
            up + "crawler_walk" + std::to_string(i) + ".png"
        ));
        run.textureNames.push_back(up + "crawler_walk" + std::to_string(i) + ".png");
        run.timeQueues.push_back(0.05);
    }

    run.textures.push_back(this->graphics.getTexture(
        up + "crawler1.png"
    ));
    run.textureNames.push_back(up + "crawler1.png");
    run.timeQueues.push_back(0.05);

    for(int i = 4; i <= 6; ++i)
    {
        run.textures.push_back(this->graphics.getTexture(
            up + "crawler_walk" + std::to_string(i) + ".png"
        ));
        run.textureNames.push_back(up + "crawler_walk" + std::to_string(i) + ".png");
        run.timeQueues.push_back(0.05);
    }

    run.textures.push_back(this->graphics.getTexture(
        up + "crawler1.png"
    ));
    run.textureNames.push_back(up + "crawler1.png");
    run.timeQueues.push_back(0.05);

    this->graphics.animateEntityMoving(enemy_id, run);


    if (!big)
        this->graphics.setHitbox(enemy_id, {125, 125});
    else
        this->graphics.setHitbox(enemy_id, {300, 300});
}

void Game::makeCustomTurret(int x, int y, int dmg, 
    double prSpeed, double fireRate, Tint tint)
{
    x *= 200;
    y *= 200;
    llint enemy_id = (this->graphics.spawnEntity("turret1.png", x, y, 
    (int)(10. * (1 + ((float)this->difficulty)*0.5)), 
    (int)(10. * (1 + ((float)this->difficulty)*0.25)), 
    Team::HOSTILE));
    this->objectNameMap["turret" + std::to_string(enemy_id)] = enemy_id;

    Animation a;

    for (int i = 1; i < 4; ++i)
    {
        a.textures.push_back(this->graphics.getTexture("turret"
        + std::to_string(i) + ".png"));
        a.textureNames.push_back("turret"
        + std::to_string(i) + ".png");
        a.timeQueues.push_back(0.1);
    }

    for (int i = 3; i > 0; --i)
    {
        a.textures.push_back(this->graphics.getTexture("turret"
        + std::to_string(i) + ".png"));
        a.textureNames.push_back("turret"
        + std::to_string(i) + ".png");
        a.timeQueues.push_back(0.1);
    }

    this->graphics.animateEntity(enemy_id, a, a);

    Animation a1, a2;

    for (int i = 1; i <= 5; ++i)
    {
        a1.textures.push_back(this->graphics.getTexture("energy_ball" + std::to_string(i) + ".png"));
        a1.textureNames.push_back("energy_ball" + std::to_string(i) + ".png");
        a1.timeQueues.push_back(0.05);
    }

    for (int i = 1; i <= 9; ++i)
    {
        a2.textures.push_back(this->graphics.getTexture("eb_exp" + std::to_string(i) + ".png"));
        a2.textureNames.push_back("eb_exp" + std::to_string(i) + ".png");
        if (i != 9)
            a2.timeQueues.push_back(0.03);
    }
    a2.timeQueues.push_back(2);
    
    this->graphics.makeEnemyRanged(enemy_id,
        a1, a2, 
        (int)((float)dmg * (1 + ((float)this->difficulty)*0.5)), 
        (prSpeed * (1 + ((float)this->difficulty)*0.2)), 
        fireRate, 
        1.5
    );

    this->graphics.turretFromRangedEntity(enemy_id);
    this->graphics.setSpecialTint(enemy_id, tint);
    this->graphics.setSpecialProjTint(enemy_id, tint);
}

void Game::spawnCrawler(int x, int y)
{
    this->makeCrawler(x, y, 100, 500, 1.f, false);
}


void Game::spawnEnhancedCrawler(int x, int y)
{
    this->makeCrawler(x, y, 120, 600, 0.3f, false);
    this->graphics.setSpecialTint(this->graphics.getLastId(), VIOLET_TINT);
}

void Game::spawnBigCrawler(int x, int y)
{
    this->makeCrawler(x, y, 300, 1000, 1.5f, true);
}

void Game::spawnRangedCrawler(int x, int y)
{
    this->spawnCrawler(x, y);
    this->graphics.setSpecialTint(this->graphics.getLastId(), RED_TINT);
    
    Animation a1, a2;

    for (int i = 1; i <= 5; ++i)
    {
        a1.textures.push_back(this->graphics.getTexture("energy_ball" + std::to_string(i) + ".png"));
        a1.textureNames.push_back("energy_ball" + std::to_string(i) + ".png");
        a1.timeQueues.push_back(0.05);
    }

    for (int i = 1; i <= 9; ++i)
    {
        a2.textures.push_back(this->graphics.getTexture("eb_exp" + std::to_string(i) + ".png"));
        a2.textureNames.push_back("eb_exp" + std::to_string(i) + ".png");
        if (i != 9)
            a2.timeQueues.push_back(0.03);
    }
    a2.timeQueues.push_back(2);
    
    this->graphics.makeEnemyRanged(this->graphics.getLastId(),
        a1, a2, 
        (int)(25. * (1 + ((float)this->difficulty)*0.5)), 
        (1.5 * (1 + ((float)this->difficulty)*0.2)), 
        0.5, 
        1.5
    );
}


void Game::spawnEnhancedRangedCrawler(int x, int y)
{
    this->spawnCrawler(x, y);
    this->graphics.setSpecialTint(this->graphics.getLastId(), GREEN_TINT);
    
    Animation a1, a2;

    for (int i = 1; i <= 5; ++i)
    {
        a1.textures.push_back(this->graphics.getTexture("energy_ball" + std::to_string(i) + ".png"));
        a1.textureNames.push_back("energy_ball" + std::to_string(i) + ".png");
        a1.timeQueues.push_back(0.05);
    }

    for (int i = 1; i <= 9; ++i)
    {
        a2.textures.push_back(this->graphics.getTexture("eb_exp" + std::to_string(i) + ".png"));
        a2.textureNames.push_back("eb_exp" + std::to_string(i) + ".png");
        if (i != 9)
            a2.timeQueues.push_back(0.03);
    }
    a2.timeQueues.push_back(2);
    
    this->graphics.makeEnemyRanged(this->graphics.getLastId(),
        a1, a2, 
        (int)(25. * (1 + ((float)this->difficulty)*0.5)), 
        (4. * (1 + ((float)this->difficulty)*0.2)), 
        0.1, 
        1.5
    );
    this->graphics.setSpecialProjTint(this->graphics.getLastId(), GREEN_TINT);
}

void Game::spawnEnemyRobot(int x, int y)
{
    x *= 200;
    y *= 200;

    llint rob_id = (this->graphics.spawnEntity("robot.png", x + 100, y + 100, 
    (int)(20. * (1 + ((float)this->difficulty)*0.5)), 
    (int)(400. * (1 + ((float)this->difficulty)*0.25)), 
    Team::HOSTILE));
    this->objectNameMap["robot" + std::to_string(rob_id)] = rob_id;
    // std::cout << "mc_id: " << std::endl;
    // std::cout << mc_id << std::endl;

    this->graphics.setSpecialTint(rob_id, LIGHTRED_TINT);

    Animation anim;

    anim.textures.push_back(this->graphics.getTexture("rob1.png"));
    anim.textureNames.push_back("rob1.png");
    anim.timeQueues.push_back(1);

    for (int j = 0; j < 100; ++j)
    {
        for(int i = 2; i <= 6; ++i)
        {
            anim.textures.push_back(this->graphics.getTexture("rob" + std::to_string(i) + ".png"));
            anim.textureNames.push_back("rob" + std::to_string(i) + ".png");
            anim.timeQueues.push_back(0.05);
        }

        anim.textures.push_back(this->graphics.getTexture("rob1.png"));
        anim.textureNames.push_back("rob1.png");
        anim.timeQueues.push_back(0.5);
    }

    this->graphics.animateTexture(rob_id, anim);
    
    Animation at;

    for(int i = 1; i <= 5; ++i) 
    {
        at.textures.push_back(this->graphics.getTexture("rob_a" + std::to_string(i) + ".png"));
        at.textureNames.push_back("rob_a" + std::to_string(i) + ".png");
    }

    at.timeQueues.push_back(0.05);
    at.timeQueues.push_back(0.03);
    at.timeQueues.push_back(0.03);
    at.timeQueues.push_back(0.03);
    at.timeQueues.push_back(0.07);
    
    at.soundNames.push_back("swordslice11.mp3");
    at.soundNames.push_back("swordslice12.mp3");
    at.soundNames.push_back("swordslice13.mp3");
    at.soundNames.push_back("swordslice14.mp3");

    at.extraSound.push_back("sword_hit2.mp3");

    this->graphics.animateEntity(rob_id, at, at);


    Animation run;

    for(int i = 1; i <= 8; ++i) 
    {
        run.textures.push_back(this->graphics.getTexture("rob_r" + std::to_string(i) + ".png"));
        run.textureNames.push_back("rob_r" + std::to_string(i) + ".png");
        run.timeQueues.push_back(0.05);
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

    this->graphics.animateEntityMoving(rob_id, run);
    this->graphics.setHitbox(rob_id, {44, 96});
}


void Game::spawnTurret(int x, int y)
{
    this->makeCustomTurret(x, y, 10, 2., 0.25, DEFAULT_TINT);
}

void Game::spawnEnhancedTurret(int x, int y)
{
    this->makeCustomTurret(x, y, 20, 3., 0.15, RED_TINT);
}

void Game::makeTrap(int x, int y)
{
    x = x * 200;
    y = y * 200;

    llint enemy_id = (this->graphics.spawnEntity("trap1.png", x, y + 75, 
    (int)(10. * (1 + ((float)this->difficulty)*0.5)), 
    100, 
    Team::HOSTILE));
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

void Game::makeTimedTrap(int x, int y)
{
    x = x * 200;
    y = y * 200;

    llint enemy_id = (this->graphics.spawnEntity("ntrap1.png", x, y, 
    (int)(200. * (1 + ((float)this->difficulty)*0.5)), 
    100, 
    Team::HOSTILE));
    this->objectNameMap["timedTrap" + std::to_string(enemy_id)] = enemy_id;
    
    this->graphics.timedTrapFromEntity(enemy_id);

    Animation anim;

    anim.textures.push_back(this->graphics.getTexture("ntrap1.png"));
    anim.textureNames.push_back("ntrap1.png");
    anim.timeQueues.push_back(1);

    this->graphics.animateTexture(enemy_id, anim);

    Animation at;

    at.textures.push_back(this->graphics.getTexture("ntrap2.png"));
    at.textureNames.push_back("ntrap2.png");
    at.timeQueues.push_back(0.05);

    for (int i = 0; i < 6; ++i)
    {
        at.textures.push_back(this->graphics.getTexture("ntrap3.png"));
        at.textureNames.push_back("ntrap3.png");
        at.timeQueues.push_back(0.02);
        at.textures.push_back(this->graphics.getTexture("ntrap4.png"));
        at.textureNames.push_back("ntrap4.png");
        at.timeQueues.push_back(0.02);
    }

    at.textures.push_back(this->graphics.getTexture("ntrap2.png"));
    at.textureNames.push_back("ntrap2.png");
    at.timeQueues.push_back(0.1);

    at.soundNames.push_back("trapzap.mp3");

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
    this->graphics.setSpecialTint(wall_id, this->wallTint);
}

// USES ABSOLUTE COORDINATES
void Game::makeCasm(const int x, const int y, const Directions& dirs, 
    const Tint& tint)
{

    llint casm_id = (this->graphics.spawnTexture("fall_"
    + std::to_string((int)dirs.left)
    + std::to_string((int)dirs.down)
    + std::to_string((int)dirs.right)
    + std::to_string((int)dirs.top) +
    ".png", x, y));
    this->objectNameMap["casm" + std::to_string(casm_id)] = casm_id;

    this->graphics.setSpecialTint(casm_id, tint);

    this->graphics.wallFromObject(casm_id);
}

// USES BLOCK COORDINATES
void Game::createCasm(int x, int y, const Directions& dirs, 
    const Tint& tint)
{
    this->makeCasm(x * 200, y * 200, dirs, tint);
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
    
    this->graphics.makeFloorFromObj(floor_id);

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



void Game::createCasmRoom(int x, int y, const int wdth, 
    const int hgth, const Directions& exits, const Tint& tint)
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
                    this->makeCasm(x + (j * 200), y - 200, {0,1,0,1}, tint);
                else
                    this->makeFloor(x + (j * 200), y - 200);
                if (!(exits.down && (
                    (wdth % 2 == 1 && j == wdth / 2) 
                    || (wdth % 2 == 0 && (j == wdth / 2 || j == wdth / 2 - 1))
                    )))
                    this->makeCasm(x + (j * 200), y + (hgth * 200), {0,1,0,1}, tint);
                else
                    this->makeFloor(x + (j * 200), y + (hgth * 200));
            }
        }
        if (!(exits.left && (
            (hgth % 2 == 1 && i == hgth / 2) 
            || (hgth % 2 == 0 && (i == hgth / 2 || i == hgth / 2 - 1))
            )))
            this->makeCasm(x - 200, y + (i * 200), {1,0,1,0}, tint);
        else
            this->makeFloor(x - 200, y + (i * 200));
        if (!(exits.right && (
            (hgth % 2 == 1 && i == hgth / 2) 
            || (hgth % 2 == 0 && (i == hgth / 2 || i == hgth / 2 - 1))
            )))
            this->makeCasm(x + (wdth * 200), y + (i * 200), {1,0,1,0}, tint);
        else
            this->makeFloor(x + (wdth * 200), y + (i * 200));
    }
    this->makeCasm(x - 200, y - 200, {1,0,0,1}, tint);
    this->makeCasm(x - 200, y + (hgth * 200), {1,1,0,0}, tint);
    this->makeCasm(x + (wdth * 200), y - 200, {0,0,1,1}, tint);
    this->makeCasm(x + (wdth * 200), y + (hgth * 200), {0,1,1,0}, tint);
}


void Game::createCasmBlock(int x, int y, const int wdth, 
    const int hgth, const Tint& tint)
{
    x = x * 200;
    y = y * 200;

    if (wdth == 0 || hgth == 0)
    {
        throw std::logic_error("Trying to create a zero-size casm block");
    }

    Directions dr = {0,0,0,0};

    for (int i = 0; i < hgth; ++i)
    {
        for (int j = 0; j < wdth; ++j)
        {
            dr = {0,0,0,0};
            if (i == 0)
                dr = {0,0,0,1};
            else if (i == hgth - 1)
                dr = {0,1,0,0};
            else if (j == 0)
                dr = {1,0,0,0};
            else if (j == wdth - 1)
                dr = {0,0,1,0};
            else
                dr = {0,0,0,0};
            
            this->makeCasm(x + j * 200, y + i * 200, dr, tint);
        }
    }
    this->makeCasm(x, y, {1,0,0,1}, tint);
    this->makeCasm(x, y + (hgth * 200) - 200, {1,1,0,0}, tint);
    this->makeCasm(x + (wdth * 200) - 200, y, {0,0,1,1}, tint);
    this->makeCasm(x + (wdth * 200) - 200, y + (hgth * 200) - 200, {0,1,1,0}, tint);
}



void Game::createCasmPath(int x, int y, const int wdth, const int hgth, 
    const Directions& exits, const Tint& tint)
{
    x *= 200;
    y *= 200;

    for (int i = 0; i < hgth; ++i)
    {
        for (int j = 0; j < wdth; ++j)
        {
            this->makeFloor(x + j * 200, y + i * 200);
            if (i == 0)
            {
                if (exits.top)
                    this->makeCasm(x + j * 200, y - 200, {0,1,0,0}, tint);
                if (exits.down)
                    this->makeCasm(x + j * 200, y + hgth * 200, {0,0,0,1}, tint);
            }
        }
        if (exits.left)
            this->makeCasm(x - 200, y + i * 200, {0,0,1,0}, tint);
        if (exits.right)
            this->makeCasm(x + wdth * 200, y + i * 200, {1,0,0,0}, tint);
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
        this->graphics.loadTextureFromRescaledImage(
            "crawler" + std::to_string(i) + ".png", 3.f);
        this->graphics.loadTextureFromImage("crawler_walk" + std::to_string(i) + ".png");
        this->graphics.loadTextureFromRescaledImage(
            "crawler_walk" + std::to_string(i) + ".png", 3.f);
    }
    this->graphics.loadTextureFromImage("crawler_walk6.png");
    this->graphics.loadTextureFromImage("crawler6.png");
    this->graphics.loadTextureFromRescaledImage("crawler_walk6.png", 3.f);
    this->graphics.loadTextureFromRescaledImage("crawler6.png", 3.f);
    this->graphics.loadTextureFromImage("rob6.png");
    this->graphics.loadTextureFromImage("rob_r6.png");
    this->graphics.loadTextureFromImage("rob_r7.png");
    this->graphics.loadTextureFromImage("rob_r8.png");

    for (int i = 1; i <= 4; ++i)
    {
        this->graphics.loadTextureFromImage("crawler_a" + std::to_string(i) + ".png");
        this->graphics.loadTextureFromRescaledImage(
            "crawler_a" + std::to_string(i) + ".png", 3.f);
        this->graphics.loadTextureFromImage("ntrap" + std::to_string(i) + ".png");
    }


    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k)
                for (int n = 0; n < 2; ++n)
                    this->graphics.loadTextureFromImage(
                        "fall_" 
                        + std::to_string(i) 
                        + std::to_string(j) 
                        + std::to_string(k) 
                        + std::to_string(n)
                        + ".png"
                    );


    for (int i = 1; i < 4; ++i)
        this->graphics.loadTextureFromImage("turret" + std::to_string(i) + ".png");
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

    this->audio.loadSoundByName("fazedstomp.mp3");

    this->audio.loadSoundByName("trapzap.mp3");
}


void Game::actOne()
{
    this->wallTint = DEFAULT_TINT;

    this->graphics.setFloorTint(DEFAULT_TINT);

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

    //Traps & heals before, enemies after
    this->makeCharacter(0, 0);

    this->spawnCrawler(54, 9);

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
    this->wallTint = DEFAULT_TINT;

    this->graphics.setFloorTint(RED_TINT);

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

    this->createHallway(25, -6, RIGHT, 2, 2);
    this->createRoom(27, -6, 2, 2, {1, 1, 0, 1});
    this->createRoom(18, -10, 1, 1, {0, 1, 1, 0});
    this->createHallway(20, -10, RIGHT, 1, 5);

    this->createRoom(25, -12, 6, 5, {1, 1, 0, 0});
    this->createHallway(11, 0, RIGHT, 1, 5);
    this->createRoom(16, 0, 1, 1, {1, 1, 1, 0});
    this->createHallway(17, 0, RIGHT, 1, 5);

    this->createHallway(22, -3, DOWN, 2, 3);

    this->createFloorBlock(22, 0, 2, 1);
    this->createWallBlock(22, 1, 3, 1);
    this->createWallBlock(24, 0, 1, 1);

    this->createHallway(16, 2, DOWN, 1, 2);
    this->createWallBlock(17, 4, 1, 3);
    this->createFloorBlock(16, 4, 1, 2);
    this->createWallBlock(16, 6, 1, 1);
    this->createRoom(7, 4, 2, 2, {0, 0, 1, 1});

    this->createHallway(10, 4, RIGHT, 2, 2);
    this->createWallBlock(14, 3, 1, 1);
    this->createHallway(15, 4, LEFT, 2, 1);

    this->createRoom(12, 4, 2, 2, {1, 1, 1, 0});
    this->createFloorBlock(15, 4, 1, 2);
    this->createWallBlock(15, 6, 1, 1);

    this->createRoom(11, 7, 4, 4, {0, 0, 1, 1}); // 4th room
    this->createHallway(27, -3, DOWN, 2, 6);

    this->createRoom(23, 3, 10, 12, {1, 0, 1, 1}); // 5th room

    this->createHallway(16, 8, RIGHT, 2, 6);

    this->createHallway(34, 8, RIGHT, 2, 4);
    this->createRoom(38, 8, 2, 2, {1, 1, 0, 0});
    this->createHallway(38, 11, DOWN, 2, 5);

    this->createRoom(38, 16, 2, 2, {0, 0, 1, 1});
    this->createRoom(41, 16, 2, 2, {1, 1, 0, 0});

    this->createHallway(41, 19, DOWN, 2, 12);
    this->createRoom(41, 31, 2, 2, {1, 0, 0, 1});
    this->createHallway(40, 31, LEFT, 2, 6);
    this->createRoom(32, 31, 2, 2, {0, 0, 1, 1});

    this->createRoom(26, 19, 14, 11, {0, 1, 0, 1}); // 14 11

    this->createRoom(32, 16, 2, 2, {1, 1, 0, 0});

    this->createHallway(31, 16, LEFT, 2, 6);

    this->createRoom(23, 16, 2, 2, {0, 1, 1, 0});

    this->createHallway(23, 19, DOWN, 2, 15);


    this->createRoom(21, 34, 6, 6, {0, 0, 0, 1});

    this->createCasmRoom(28, 22, 3, 5, {0, 0, 0, 1}, RED_TINT);
    this->createCasm(30, 21, {1,1,0,1}, RED_TINT);
    this->createCasm(28, 21, {0,1,1,1}, RED_TINT);
    this->createCasmRoom(35, 22, 3, 5, {0, 0, 0, 1}, RED_TINT);
    this->createCasm(37, 21, {1,1,0,1}, RED_TINT);
    this->createCasm(35, 21, {0,1,1,1}, RED_TINT);

    //this->createRoom(32, 18, 2, 2, {1, 1, 0, 0});

    //this->createHallway(13, 0, RIGHT, 1, 5);


    this->makeTimedTrap(5, 0);
    this->makeTimedTrap(16, 0);

    this->makeTimedTrap(18, -9);

    this->makeTimedTrap(24, -10);

    this->makeTrap(6, 2);
    this->makeTrap(9, 2);
    this->makeTrap(9, -2);
    this->makeTrap(6, -2);

    for (int i = 0; i < 8; ++i)
    {
        this->makeTrap(7 + i, -9);
    }

    this->makeTrap(23, -4);
    this->makeTrap(22, -2);
    this->makeTrap(23, 0);

    this->makeTimedTrap(21, 0);

    // for (int i = 0; i < 5; ++i)
    // {
    //     for (int j = 0; j < 12; ++j)
    //     {
    //         this->makeTimedTrap(23 + i * 2 + (j % 2), 3 + j);
    //     }
    // }

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            this->makeTimedTrap(33 + i, 8 + j);
        }
    }


    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 7; ++j)
        {
            this->makeTrap(27 + i, -4 + j);
        }
    }

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            this->makeTimedTrap(23 + i * 2 + (j % 2), 4 + j);
        }
    }

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            this->makeTimedTrap(23 + i * 2 + (j % 2), 11 + j);
        }
    }

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            this->makeTimedTrap(41 + i, 24 + j);
        }
    }

    for (int j = 0; j < 2; ++j)
    {
        this->makeTimedTrap(37, 31 + j);
    }


    this->makeHeal(29, -12);
    this->makeHeal(30, -12);
    this->makeHeal(30, -11);

    this->makeHeal(23, 3);
    this->makeHeal(32, 14);
    this->makeHeal(23, 14);
    this->makeHeal(32, 3);

    this->makeHeal(40, 16);
    this->makeHeal(40, 17);

    this->makeHeal(39, 24);
    this->makeHeal(26, 24);

    this->makeHeal(23, 24);
    this->makeHeal(24, 24);

    //Traps & heals before, enemies after
    this->makeCharacter(0, 0);


    this->makeEnemy(8, -2);
    this->makeEnemy(8, -2);
    this->makeEnemy(8, 2);
    this->makeEnemy(8, 2);

    this->makeEnemy(8, -8);
    this->makeEnemy(8, -8);

    this->makeEnemy(13, -8);
    this->makeEnemy(13, -8);

    for (int i = 0; i < 10; ++i)
        this->makeEnemy(18, -5);
    
    this->spawnCrawler(18, -5);

    for (int i = 0; i < 2; ++i)
        this->spawnCrawler(27, -10);

    for (int i = 0; i < 6; ++i)
        this->makeEnemy(22, -6);

    for (int i = 0; i < 4; ++i)
        this->makeEnemy(12, 4);
    
    for (int i = 0; i < 8; ++i)
        this->makeEnemy(12, 8);

    this->spawnCrawler(12, 8);

    this->spawnRangedCrawler(30, 26);
    this->spawnRangedCrawler(30, 26);

    

    this->spawnRangedCrawler(35, 26);
    this->spawnRangedCrawler(35, 26);

    for (int i = 0; i < 4; ++i)
        this->spawnEnemyRobot(24, 37);

    this->spawnRangedCrawler(25, 38);
    this->spawnRangedCrawler(21, 38);

    for (int i = 0; i < 3; ++i)
        this->spawnCrawler(32, 24);

    this->audio.stopAmbient();

    this->audio.playAmbient("ost1.mp3", 0.05);
}

void Game::actThree()
{
    this->graphics.setFloorTint(YELLOW_TINT);
    this->wallTint = DARK_ORANGE_TINT;

    this->createRoom(-3, -3, 6, 6, {1,1,1,1}); //1st room

    this->createHallway(-1, 4, DOWN, 2, 2); 

    this->createRoom(-5, 6, 10, 10, {0,0,0,1});
    this->createCasmBlock(-5, 8, 10, 8, YELLOW_TINT);

    this->createFloorBlock(-1, -12, 2, 8);

    this->createFloorBlock(-3, -10, 1, 3);
    this->createFloorBlock(2, -10, 1, 3);

    this->createCasmPath(4, -1, 2, 2, {0,1,1,0}, YELLOW_TINT);
    this->createCasmPath(4, -4, 2, 3, {0,0,1,0}, YELLOW_TINT);
    this->createCasmPath(4, -12, 2, 8, {1,0,1,0}, YELLOW_TINT);

    this->createCasmPath(-6, -1, 2, 2, {1,1,0,0}, YELLOW_TINT);
    this->createCasmPath(-6, -4, 2, 3, {1,0,0,0}, YELLOW_TINT);
    this->createCasmPath(-6, -12, 2, 8, {1,0,1,0}, YELLOW_TINT);

    for (int i = -12; i <= -5; ++i)
    {
        this->createCasm(-4, i, {1,0,1,0}, YELLOW_TINT);
        this->createCasm(-2, i, {1,0,1,0}, YELLOW_TINT);
        this->createCasm(1, i, {1,0,1,0}, YELLOW_TINT);
        this->createCasm(3, i, {1,0,1,0}, YELLOW_TINT);
    }

    this->createCasm(2, -7, {0,0,0,1}, YELLOW_TINT);
    this->createCasm(2, -11, {0,1,0,0}, YELLOW_TINT);

    this->createCasm(-3, -7, {0,0,0,1}, YELLOW_TINT);
    this->createCasm(-3, -11, {0,1,0,0}, YELLOW_TINT);

    this->createFloorBlock(2, -12, 1, 8);
    this->createFloorBlock(-3, -12, 1, 8);

    this->createCasm(-4, -12, {1,0,0,1}, YELLOW_TINT);
    this->createCasm(-3, -12, {0,1,0,1}, YELLOW_TINT);
    this->createCasm(-2, -12, {0,0,1,1}, YELLOW_TINT);

    this->createCasm(1, -12, {1,0,0,1}, YELLOW_TINT);
    this->createCasm(2, -12, {0,1,0,1}, YELLOW_TINT);
    this->createCasm(3, -12, {0,0,1,1}, YELLOW_TINT);

    this->createCasmPath(-6, -18, 12, 6, {1,0,1,0}, YELLOW_TINT);

    for (int i = 0; i < 3; ++i)
        this->createCasmBlock(-4 + 3 * i, -16, 2, 2, YELLOW_TINT);

    for (int c = 0; c < 2; c++)
    {
        for (int i = 0; i < 4; ++i)
            this->spawnEnhancedEnemy(-2 + c * 3, -16);
        this->spawnEnhancedCrawler(-5 + c * 9, -16);
        this->spawnRangedCrawler(-3 + c * 5, -17);
        this->createWallBlock(-6 + c * 7, -19, 5, 1);
    }

    this->createHallway(-1, -18, TOP, 2, 8);

    for (int i = 0; i < 3; ++i)
    {
        this->createCasm(-1 + i % 2, -21 - 2 * i, 
        {(bool)(i % 2), 1, (bool)((i + 1) % 2), 1}, YELLOW_TINT);
    }
    this->makeTimedTrap(-1, -24);
    this->makeTimedTrap(0, -24);
    this->makeTimedTrap(0, -25);
    this->makeTimedTrap(-1, -22);
    this->makeTimedTrap(0, -22);
    this->makeTimedTrap(0, -21);

    this->createCasm(-2, -23, {1,1,1,1}, YELLOW_TINT);
    this->createCasm(-3, -24, {0,1,0,0}, YELLOW_TINT);
    this->createCasm(-4, -23, {0,0,1,0}, YELLOW_TINT);
    this->createCasm(-3, -22, {0,0,0,1}, YELLOW_TINT);
    this->spawnEnhancedTurret(-3, -23);

    this->createCasmPath(-2, -30, 4, 4, {1,0,0,1}, YELLOW_TINT);

    this->createHallway(2, -29, RIGHT, 2, 8);

    this->createRoom(10, -31, 6, 6, {1,1,0,0});

    this->createHallway(12, -25, DOWN, 2, 4);

    this->createRoom(10, -21, 6, 8, {0,1,0,1});

    for (int i = 0; i < 2; ++i)
    {
        this->createCasm(12, -20 + i * 5, {1,1,0,1}, YELLOW_TINT);
        this->createCasm(13, -20 + i * 5, {0,1,1,1}, YELLOW_TINT);
    }

    this->createWallBlock(12, -18, 2, 2);

    this->createCasmPath(12, -12, 2, 6, {1,0,1,0}, YELLOW_TINT);

    this->createRoom(10, -5, 6, 8, {0,1,0,1});

    this->createCasm(10, -3, {0,1,0,1}, YELLOW_TINT);
    this->createCasm(11, -3, {0,0,1,1}, YELLOW_TINT);
    this->createCasm(11, -2, {1,0,1,0}, YELLOW_TINT);
    this->createCasm(11, -1, {1,0,1,0}, YELLOW_TINT);
    this->createCasm(11, 0, {0,1,1,0}, YELLOW_TINT);
    this->createCasm(10, 0, {0,1,0,1}, YELLOW_TINT);
    this->spawnTurret(10, -2);
    this->spawnTurret(10, -1);

    this->createCasm(15, -3, {0,1,0,1}, YELLOW_TINT);
    this->createCasm(14, -3, {1,0,0,1}, YELLOW_TINT);
    this->createCasm(14, -2, {1,0,1,0}, YELLOW_TINT);
    this->createCasm(14, -1, {1,0,1,0}, YELLOW_TINT);
    this->createCasm(14, 0, {1,1,0,0}, YELLOW_TINT);
    this->createCasm(15, 0, {0,1,0,1}, YELLOW_TINT);
    this->spawnTurret(15, -2);
    this->spawnTurret(15, -1);

    this->createHallway(12, 3, DOWN, 2, 14);
    this->createRoom(10, 17, 6, 6, {0,0,0,1});

    //Traps & heals before, enemies after
    this->makeCharacter(0, 0);


    this->spawnBigCrawler(10, -29);

    for (int j = 0; j < 2; ++j)
    {
        for (int i = 0; i < 4; ++i)
            this->spawnCrawler(11 + 3 * j, -18);
        this->spawnRangedCrawler(11 + 3 * j, -17);
        this->spawnRangedCrawler(11 + 3 * j, -17);
        this->spawnEnhancedEnemy(12 + j, -13);
        this->spawnEnhancedEnemy(12 + j, -13);
        this->spawnEnhancedEnemy(12 + j, -13);
    }

    for (int j = 0; j < 4; ++j)
        this->spawnEnhancedCrawler(12, -1);

    this->spawnEnhancedRangedCrawler(11, 19);
    this->spawnEnhancedRangedCrawler(14, 19);


    this->spawnTurret(2, -8);
    this->spawnTurret(2, -10);

    this->spawnTurret(-3, -8);
    this->spawnTurret(-3, -10);

    this->audio.stopAmbient();

    this->audio.playAmbient("ost1.mp3", 0.05);
}