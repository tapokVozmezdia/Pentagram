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
    #define CHARACTER_SPEED 10.f // DEFAULT = 9.f
#endif

#ifndef ENEMY_SPEED
    #define ENEMY_SPEED 6.f
#endif

#ifndef ENEMY_VISIBILITY
    #define ENEMY_VISIBILITY 450
#endif

#ifndef PROJECTILE_SPEED
    #define PROJECTILE_SPEED 15.f
#endif

#ifndef NOISE_FREQUENCY
    #define NOISE_FREQUENCY 2.5
#endif

#ifndef NOISE_LENGTH
    #define NOISE_LENGTH 1.4
#endif

#ifndef TRAP_CD
    #define TRAP_CD 2.f
#endif

#ifndef HOSTILITY_BREAK
    #define HOSTILITY_BREAK FPS * 3.
#endif

#ifndef SHIELD_REFRESH_RATE
    #define SHIELD_REFRESH_RATE FPS / 10.
#endif

#ifndef HP_BUFF
    #define HP_BUFF 400
#endif

#ifndef SH_BUFF
    #define SH_BUFF 200 
#endif

#ifndef AT_BUFF
    #define AT_BUFF 16
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
        if (list->empty() || list == nullptr)
            return;
        auto tmp = *(list->begin());
        list->pop_front();
        list->push_back(tmp);
    }

    template <class T1>
    void shiftVectorL(std::vector<T1>* vector)
    {
        if (vector->empty() || vector == nullptr)
            return;
        auto tmp = *(vector->begin());
        vector->erase(vector->begin());
        vector->push_back(tmp);
    }
    template <class T1>
    void shiftListR(std::list<T1>* list)
    {
        if (list->empty() || list == nullptr)
            return;
        auto tmp = *(list->end());
        list->pop_back();
        list->push_front(tmp);
    }

    template <class T1>
    void shiftVectorR(std::vector<T1>* vector)
    {
        if (vector->empty() || vector == nullptr)
            return;
        auto tmp = *(vector->end());
        vector->erase(vector->end());
        vector->push_front(tmp);
    }
    template <class T1>
    T1 getElementsSum(std::vector<T1>* vector)
    {
        // std::cout << "SUM: " << std::endl;
        if (vector->empty()  || vector == nullptr)
            return 0;
        T1 sum = 0;
        for (auto i = vector->begin(); i != vector->end(); ++i)
        {
            sum += (*i);
        }
        // std::cout << sum << std::endl;
        return sum;
    }
}


typedef enum
{
    DEFAULT_TINT = 0,
    RED_TINT,
    YELLOW_TINT,
    DARK_ORANGE_TINT,
    GREEN_TINT,
    BLUE_TINT,
    VIOLET_TINT,
    DARK_PURPLE_TINT,
    LIGHTRED_TINT
} Tint;


namespace enumHandler
{
    Color tintToColor(const Tint& tnt);
}



typedef enum
{
    MAIN = 0,
    ALLY,
    NEUTRAL,
    HOSTILE
} Team;


typedef enum 
{
    LEFT = 0,
    DOWN,
    RIGHT,
    TOP
} SingularDirection;


struct Directions
{
    bool left; 
    bool down; 
    bool right; 
    bool top; 
};


struct Button
{
    llint button_id;
    std::string text;
    bool visible;
    bool isUpgrade = false;
    bool isDifficulty = false;
    uint width = 250;
    uint height = 50;
    bool is_clicked = false;
    Vector2 position = {0, 0};
    std::list<bool> ButtonVisibilityTrace;
    Button* parent = nullptr;
    bool hovered = false;
    std::string upgradeText = "null";
};

struct Animation
{
    std::list<Texture2D*>  textures;
    std::list<std::string> textureNames;
    std::vector<double> timeQueues;
    std::list<std::string> soundNames;
    std::list<std::string> extraSound;
    Texture2D* firstTexture = nullptr;
    uint curTime = 0;
};


struct Hitbox
{
    int width;
    int height;
};


struct ProjectileHitProfile
{
    int damageOnHit = 0;
    bool hasHit = false;
    bool corrosive = false;
}; 


struct Flipper
{
    int horz = 1;
    int vert = 1;
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
    bool floor = false;
    bool isWall = false;
    bool nL = false; // Crutch for inheritance, for Entities
    bool nLP = false; 
    bool isButton = false;
    bool mark = false;
    bool isFlipped = false;
    uint EXTRA = 0;

    Tint specialTint = DEFAULT_TINT;
    Flipper flips;

    std::unique_ptr<ProjectileHitProfile> hitProfile = nullptr;
};


struct Projectile : GameObject
{
    int damage;
    bool exploded;
    std::shared_ptr<Animation> hitAnimation = nullptr;
    Vector2 target;
    double speedModifier = 1;
    bool isExplosive = true;
    Tint hitTint = DEFAULT_TINT;
};



struct RangedEntityProfile
{
    std::shared_ptr<Animation> flightAnim = nullptr;
    std::shared_ptr<Animation> hitAnim = nullptr;
    double fireRate = 1;
    double range = 600;
    double projSpeed = 1;
    int dmg = 0;
    uint rangedTimer = 0;
    Tint pr_tint = DEFAULT_TINT;
};



// Basic Entity type
struct Entity : GameObject
{   
    int baseDamage;
    int baseHP;
    int curHP;
    int baseShield = 0;
    int curShield = 0;
    std::shared_ptr<Animation> attackAnimation = nullptr;
    std::shared_ptr<Animation> getHitAnimation = nullptr;
    std::shared_ptr<Animation> movingAnimation = nullptr;
    bool isAttacking = false;
    uint curSt = 0;
    Vector2 * target = nullptr;
    Team team;
    bool isTrap = false;
    bool isTimedTrap = false;
    bool isTurret = false;
    bool hasMoved = false;
    bool movingFlag = false;
    uint noise = 0;
    double noise_length = 0;
    Vector2 momentum;
    Hitbox hitbox;
    bool didDamage = false;

    std::unique_ptr<RangedEntityProfile> rangedProfile = nullptr;
};
// TAK PLOHO


// NADO TAK
// struct info
// {
        // add vars
// };


// struct obj
// {
//     // base vars
//    
//     std::unique_ptr<info> inf = nullptr;
// };


namespace ObjectHandler
{
    const Vector2 getCenter(GameObject* obj);
    bool collided(GameObject* obj_1, GameObject* obj_2);
    bool hitBoxCollided(Entity* obj_1, Entity* obj_2);
    double measureDistance(const Vector2& u, const Vector2& v);
    const Vector2 getVectorDiff(const Vector2& u, const Vector2& v);
    const Vector2 getVectorWithLength(const Vector2& u, double lngth); // Returns same vector with different length
}




struct ProgressTrack
{
    int HP_BUFFS = 0;
    int SH_BUFFS = 0;
    int AT_BUFFS = 0;
};


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