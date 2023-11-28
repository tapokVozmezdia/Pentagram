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


        void loadTextures();


        void actOne();
        void makeCharacter(const int x, const int y);
        void makeEnemy(int x, int y);
        void spawnRoboss(int x, int y);
        void makeTrap(int x, int y);
        void makeHeal(int x, int y);
        void makeWall(const int x, const int y);
        void makeFloor(const int x, const int y);

        void launchEnergyBall(int x, int y, int t_x, int t_y);

        void createRoom(int x, int y, const int wdth, const int hgth, const Directions& exits);
        void createWallBlock(int x, int y, const int wdth, const int hgth); // Just a box with no entrance
        void createFloorBlock(int x, int y, const int wdth, const int hgth); // -||- for floor
        void createHallway(int x,int y, const SingularDirection dir, const uint wdth, const uint length);
};