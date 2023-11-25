// Some definitions that are used throughout the project 

#pragma once

#ifndef MISC
#define MISC

#include "raylib.h" // to install raylib run the first command in build-instrutions.txt

#include <iostream>
#include <format>
#include <stdexcept>
#include <list>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cmath>

#ifndef WIDTH
    #define WIDTH 1152
#endif

#ifndef HEIGHT
    #define HEIGHT 648
#endif

#ifndef FPS
    #define FPS 60
#endif

#ifndef CHARACTER_SPEED
    #define CHARACTER_SPEED 9.f
#endif

#ifndef ENEMY_SPEED
    #define ENEMY_SPEED 6.f
#endif

#ifndef ENEMY_VISIBILITY
    #define ENEMY_VISIBILITY 450
#endif

#ifndef NOISE_FREQUENCY
    #define NOISE_FREQUENCY 2.5
#endif

#ifndef NOISE_LENGTH
    #define NOISE_LENGTH 1.4
#endif

typedef unsigned int uint;
typedef long long int llint;

namespace IdCounter
{
    static llint freeId = 1;
    llint getFreeId();
}

namespace ContainerHandler
{
    template <class T1>
    void shiftListL(std::list<T1>* list)
    {
        if (list->empty())
            return;
        auto tmp = *(list->begin());
        list->pop_front();
        list->push_back(tmp);
    }

    template <class T1>
    void shiftVectorL(std::vector<T1>* vector)
    {
        if (vector->empty())
            return;
        auto tmp = *(vector->begin());
        vector->erase(vector->begin());
        vector->push_back(tmp);
    }
}


typedef enum
{
    MAIN = 0,
    ALLY,
    NEUTRAL,
    HOSTILE
} Team;


struct Animation
{
    std::list<Texture2D*>  textures;
    std::list<std::string> textureNames;
    std::vector<double> timeQueues;
    uint curTime = 0;
};


// Basic Game Object type
struct GameObject
{
    Texture2D* texture; // Pointer to the base texture
    std::string textureName; // Relative name of the texture in the project
    Vector2 coordinates; // Top-left point 
    llint objectId; // Unique id  
    std::shared_ptr<Animation> animation = nullptr;
    bool collision; // Is the object collided
    bool isWall = false;
    bool nL = false; // Crutch for inheritance 
};

// Basic Entity type
struct Entity : GameObject
{   
    uint baseDamage;
    uint baseHP;
    int curHP;
    std::shared_ptr<Animation> attackAnimation = nullptr;
    std::shared_ptr<Animation> getHitAnimation = nullptr;
    bool isAttacking = false;
    uint curSt = 0;
    Vector2 * target = nullptr;
    Team team;
    bool isTrap = false;
    uint noise = 0;
    double noise_length = 0;
    Vector2 momentum;
};


namespace ObjectHandler
{
    const Vector2 getCenter(GameObject* obj);
}


// Basic structure of Abelian group, just thought it would be a fun solution :)
class AbelianGroup
{
    public:
        AbelianGroup(const std::vector<uint>& table);
        uint product(const uint a, const uint b) const;
        const uint cardinality() const;
    private:
        uint _сardinality;
        std::vector<uint> CayleyTable;
};
/* 
P.S. This class in general case actually represents a commutative groupoid,
since not all conditions for it being a group are checked in the constructor
(existence of the neutral element and associativity)
However, I don't see the need to define a fully-complete Abelian Group
because the code will descend into a complete bedlam,
so it's recommended to create and employ instances of this class 
with caution (or don't at all)
*/


#endif // MISC