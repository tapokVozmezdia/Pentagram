#pragma once

#include "controls.hpp"

class Game
{
    public:

        Game(uint screenWidth, uint screenHeight);
        Game(uint fps, uint screenWidth, uint screenHeight);
        ~Game();

        void run();

    private:

        GraphicsHandler graphics;    

        Menu menu;

        std::unordered_map<std::string, llint> objectNameMap;

        // llint mainCharachetrId;

        // std::list<llint> enemyId;

        uint screenWidth, screnHeight;
        uint starNum = 0;

        void actOne();
        void makeEnemy(const int x, const int y);
        void makeTrap(const int x, const int y);
        void makeWall(const int x, const int y);
        void makeFloor(const int x, const int y);
};