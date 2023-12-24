// All that concerns textures and screen is mostly here  

#pragma once

#include "audioHandler.hpp"
#include "statRecorder.hpp"

class GraphicsHandler
{
    public:
        GraphicsHandler();

        ~GraphicsHandler();

        void linkAudio(AudioHandler* audio_ptr);

        void syncAudio();

        void linkProgressTracker(ProgressTrack* tracker, std::vector<bool>* lvls, 
            int* diff, Recorder* rec);

        // Loads a texture from a specific folder by image name
        void loadTextureFromImage(const std::string& imgName); // Custom method, don't raylib one!!!
        void loadTextureFromRescaledImage(const std::string& imgName, float scale);

        Texture2D* getTexture(const std::string& tName); // Returns texture from texture map by name

        //Spawns basic GameObject type object
        llint spawnTexture(const std::string& txtr, const Vector2& pos); 
        llint spawnTexture(const std::string& txtr, int p_x, int p_y);


        //Spawns Entity type object
        llint spawnEntity(const std::string& txtr, Vector2& pos, double dmg, double hp, Team tm); 
        llint spawnEntity(const std::string& txtr, int p_x, int p_y, double dmg, double hp, Team tm);


        void makeFloorFromObj(llint t_id);

        void setFloorTint(const Tint& tint);


        void setHitbox(llint t_id, const Hitbox& hbox);
        void setHitbox(llint t_id, const int width, const int height);


        void setBaseShield(const llint t_id, const int shield);


        void setFlips(const llint t_id, const Flipper& fl);


        //Transforms a GameObject into a wall
        void wallFromObject(const llint object_id);

        //Transforms an entity into a trap
        void trapFromEntity(const llint object_id);

        void timedTrapFromEntity(const llint object_id);

        void turretFromRangedEntity(const llint object_id);

        void setSpecialTint(const llint object_id, const Tint& tint);

        void makeEnemyRanged(const llint t_id, const Animation& flight, const Animation& hit,
            int dmge, double speed, double rate, double rng);

        void setSpecialProjTint(const llint t_id, Tint tint);


        //Deletes GameObject or inherited object by id  
        void deleteTexture(const llint object_id);


        void animateTexture(const llint t_id, const Animation& anim);
        void animateEntity(const llint t_id, const Animation& attack, const Animation& hit);
        void animateEntityMoving(const llint t_id, const Animation& move);



        void triggerAttack(const llint t_id);

        llint fireProjectile(const std::string& txtr, const Vector2& src,
            const Vector2& trgt, const int dmg, const double speed,
            const Animation& flight, const Animation& hit
        );

        llint fireTestProjectileFromObj(const llint t_id
        );

        //Makes enemies attack the player
        void enemyHostilityTriggerOn(); 
        //Stops enemies from attacking the player
        void enemyHostilityTriggerOff(); 


        const llint getLastId() const; // Returns id of the last spawnde texture


        void moveTextureAbs(const llint t_id, int dx, int dy);
        void moveTextureAbs(const llint t_id, Vector2& ds);

        void moveTextureDelta(const llint t_id, float dx, float dy);
        void moveTextureDelta(const llint t_id, const Vector2& ds);

        bool isFlipped(const llint t_id);
        void flipTexture(const llint t_id);

        void playAnimationOnce(const Vector2& pos, const Animation& anim);


        void centerCamera(llint t_id); // Centers active camera on an object with given id 

        void clearAll(); // removes all textures (irreversable)

        void drawButtons();
        void linkButtons(std::list<Button>* _buttons);
        void buttonClickCheck();

        void perform();

        void flagTheStart();
        void flagVictory();

        // Returns false in the camera center is lost, else true
        bool checkPulse(); 

        bool checkVictory();

        void setCurLvl(const uint lvl);

    private:

        void run();


        void mainObjectCycle();


        // Sets collision for all objects in objectList
        void collisionCheck(GameObject* i, GameObject* j);
        void resetCollisions();
        void resetMovement();

        void animationCheck();

        void fightCheck(GameObject* i, GameObject* j);

        //Makes all the enemies charge and attack the player in the visibility radius
        void enemyHostile(Entity* i, Entity* j);

        void afterFightCheck();

        void checkRangedEnemy(Entity* enemy, Entity* player);
        void projectileHit(Entity* i, GameObject* j);

        void buttonManage(Button* button);

        void drawStats();

        Camera2D activeCamera {0};
        Vector2* cameraTarget = nullptr;
        llint targetId;

        llint trapTimer = 0;
        bool triggerTraps = false;

        llint damageTimer = 0;

        llint shieldTimer = 0;

        int enemyNum = 0;

        int* difficulty = nullptr;

        bool buttonFlag = false;

        bool startFlag = false;

        bool statFlag = false;

        bool victoryFlag = false;

        Tint curTint = DEFAULT_TINT;
        
        Button* buttonPressed = nullptr;

        ProgressTrack* linkedTracker = nullptr;
        std::vector<bool>* passedLvls = nullptr;

        uint curLvl;

        std::list<Button*> buttonList;
        std::list<GameObject*> objectList;
        std::list<llint> wallIndexList;
        
        std::unordered_map<llint, GameObject*> objectMap;
        std::unordered_map<std::string, Texture2D> textureMap;

        std::list<llint> toDeleteList;

        bool hostilityFlag = true;

        AudioHandler* audio = nullptr;
        Recorder* records = nullptr;
};