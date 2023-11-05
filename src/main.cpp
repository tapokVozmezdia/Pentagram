#include "game.hpp"

int main()
{
    Game game(FPS, WIDTH, HEIGHT);

    game.run();

    // std::vector<GameObject*> list;

    // Entity e;
    // e.texture = nullptr;
    // e.textureName = "none";
    // e.coordinates = {0,0};
    // e.objectId = 0;
    // e.collision = false;
    // e.baseDamage = 10;
    // e.baseHP = 10000;

    // GameObject o;
    // o.texture = nullptr;
    // o.textureName = "no none";
    // o.coordinates = {-7,-6};
    // o.objectId = 3;
    // o.collision = true;

    // GameObject o1 = o;

    // list.push_back(&o);
    // list.push_back(&e);
    // list.push_back(&o1);

    // std::cout << ((Entity*)(list[1]))->baseHP << " "
    //  << ((Entity*)(list[1]))->baseDamage << std::endl;

    

    return 0;
}