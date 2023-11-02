#include "penta_mc.hpp"

llint IdCounter::getFreeId()
{
    freeId++;
    return freeId - 1;
}

GameObject::~GameObject()
{
    delete this->animation;
}