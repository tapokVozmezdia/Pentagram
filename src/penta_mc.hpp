// Some defines that are used throughout the project 

#pragma once

#ifndef MC
#define MC

#include "raylib.h" // to install raylib run the first command in build-instrutions.txt

#include <iostream>
#include <format>
#include <stdexcept>
#include <list>
#include <vector>
#include <unordered_map>

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
    #define CHARACTER_SPEED 4.5f
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
    Animation* animation = nullptr;
    bool collision; // Is the object collided
    ~GameObject();
};

#endif // MC