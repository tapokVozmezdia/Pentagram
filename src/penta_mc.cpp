#include "penta_mc.hpp"

llint IdCounter::getFreeId()
{
    freeId++;
    return freeId - 1;
}

AbelianGroup::AbelianGroup(const std::vector<uint>& table)
{
    // Check if size of given table is a square of an integer
    if (std::floor(sqrt(static_cast<float>(table.size()))) != sqrt(static_cast<float>(table.size())))
    {
        throw std::logic_error("Invalid Cayley Table Size");
    }
    uint newSize = static_cast<uint>(sqrt(static_cast<float>(table.size())));
    for(int i = 0; i < newSize; ++i)
    {
        for (int j = i; j < newSize; ++j)
        {
            if (i == j)
                continue;
            if (table[j + i * newSize] != table[i + j * newSize])
            {
                throw std::logic_error("Non-Abelian Caley Table given to Abelian Group constructor");
            }
        }
    }
    this->CayleyTable = table;
    this->_сardinality = newSize;
}

uint AbelianGroup::product(const uint a, const uint b) const
{
    return this->CayleyTable[a + b * this->_сardinality];
}

const uint AbelianGroup::cardinality() const
{
    return this->_сardinality;
}

const Vector2 ObjectHandler::getCenter(GameObject* obj)
{
    return Vector2{
        obj->coordinates.x + (obj->texture->width / 2),
        obj->coordinates.y + (obj->texture->height / 2)
    };
}

bool ObjectHandler::collided(GameObject* obj_1, GameObject* obj_2)
{
    if (obj_1->coordinates.x < obj_2->coordinates.x + obj_2->texture->width
    && obj_1->coordinates.x + obj_1->texture->width > obj_2->coordinates.x
    && obj_1->coordinates.y < obj_2->coordinates.y + obj_2->texture->height
    && obj_1->coordinates.y + obj_1->texture->height > obj_2->coordinates.y
    )
    {
        // std::cout << "ATTACK!" << std::endl;
        return true;
    }
    return false;
}