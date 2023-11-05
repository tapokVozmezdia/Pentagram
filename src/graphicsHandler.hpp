// All that concerns textures and screen is mostly here  

#pragma once

#include "penta_mc.hpp"

class GraphicsHandler
{
    public:
        GraphicsHandler();

        ~GraphicsHandler();

        // Loads a texture from a specific folder by image name
        void loadTextureFromImage(const std::string& imgName); // Custom method, don't raylib one!!!

        Texture2D* getTexture(const std::string& tName); // Returns texture from texture map by name


        //Spawns basic GameObject type object
        llint spawnTexture(const std::string& txtr, Vector2& pos); 
        llint spawnTexture(const std::string& txtr, int p_x, int p_y);


        //Spawns Entity type object
        llint spawnEntity(const std::string& txtr, Vector2& pos, double dmg, double hp); 
        llint spawnEntity(const std::string& txtr, int p_x, int p_y, double dmg, double hp);


        void animateTexture(const llint t_id, Animation& anim);
        void animateEntity(const llint t_id, Animation& attack, Animation& hit);

        void triggerAttack(const llint t_id);


        const llint getLastId() const; // Returns id of trhe last spawnde texture


        void moveTextureAbs(const llint t_id, int dx, int dy);
        void moveTextureAbs(const llint t_id, Vector2& ds);

        void moveTextureDelta(const llint t_id, int dx, int dy);
        void moveTextureDelta(const llint t_id, Vector2& ds);


        void centerCamera(llint t_id); // Centers active camera on an object with given id 

        void clearAll(); // removes all textures (irreversable)

        void perform();


    private:
        void run();

        // Sets collision for all objects in objectList
        void collisionCheck();
        void resetCollisions();

        void animationCheck();

        Camera2D activeCamera {0};
        Vector2 * cameraTarget = nullptr;

        std::list<GameObject*> objectList;
        
        std::unordered_map<llint, GameObject*> objectMap;
        std::unordered_map<std::string, Texture2D> textureMap;
};