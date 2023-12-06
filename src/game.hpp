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
        AudioHandler audio;   

        Menu menu;

        std::unordered_map<std::string, llint> objectNameMap;

        // llint mainCharachetrId;

        // std::list<llint> enemyId;

        uint screenWidth, screnHeight;
        uint starNum = 0;


        ProgressTrack pTrack;

        uint levelLaunched;

        std::vector<bool> levelsPassed;

        void loadTextures();

        void loadSounds();


        void actOne();
        void actTwo();


        void makeCharacter(const int x, const int y);
        void makeEnemy(int x, int y);
        void spawnCrawler(int x, int y);
        void spawnRangedCrawler(int x, int y);
        void spawnEnemyRobot(int x, int y);
        void makeTrap(int x, int y);
        void makeTimedTrap(int x, int y);
        void makeHeal(int x, int y);
        void makeWall(const int x, const int y);
        void makeFloor(const int x, const int y);
        void makeCasm(const int x, const int y, const Directions& dirs, const Tint& tint);

        void launchEnergyBall(int x, int y, int t_x, int t_y);

        void createRoom(int x, int y, const int wdth, const int hgth, const Directions& exits);
        void createWallBlock(int x, int y, const int wdth, const int hgth); // Just a box with no entrance
        void createFloorBlock(int x, int y, const int wdth, const int hgth); // -||- for floor
        void createHallway(int x,int y, const SingularDirection dir, const uint wdth, const uint length);

        void createCasm(int x, int y, const Directions& dirs, const Tint& tint);
        void createCasmRoom(int x, int y, const int wdth, const int hgth, const Directions& exits, const Tint& tint);
        void createCasmBlock(int x, int y, const int wdth, const int hgth, const Tint& tint);
        void createCasmPath(int x,int y, const SingularDirection dir, const uint length, const Tint& tint);
};