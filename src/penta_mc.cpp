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
        return true;
    }
    return false;
}

bool ObjectHandler::hitBoxCollided(Entity* obj_1, Entity* obj_2)
{

    Vector2 cent1 = ObjectHandler::getCenter(obj_1);
    Vector2 cent2 = ObjectHandler::getCenter(obj_2);

    // std::cout << "HITBOX COLLISION!" << std::endl;

    if (cent1.x + obj_1->hitbox.width / 2 > cent2.x - obj_2->hitbox.width / 2
    && cent1.x - obj_1->hitbox.width / 2 < cent2.x + obj_2->hitbox.width / 2
    && cent1.y + obj_1->hitbox.height / 2 > cent2.y - obj_2->hitbox.height / 2
    && cent1.y - obj_1->hitbox.height / 2 < cent2.y + obj_2->hitbox.height / 2
    )
    {
        // std::cout << "OBJ 1 cent: " << cent1.x << " " << cent1.y << std::endl;
        // std::cout << "W: " << obj_1->hitbox.width <<
        //     " " << "H: " <<  obj_1->hitbox.height << std::endl;

        // std::cout << "OBJ 2 cent: " << cent2.x << " " << cent2.y << std::endl;
        // std::cout << "W: " << obj_2->hitbox.width <<
        //     " " << "H: " <<  obj_2->hitbox.height << std::endl;

        // std::cout << "HITBOX COLLISION!" << std::endl;

        return true;
    }
    return false;
}


double ObjectHandler::measureDistance(const Vector2& u, const Vector2& v)
{
    return sqrt((v.x - u.x) * (v.x - u.x) + (v.y - u.y) * (v.y - u.y));
}

//U - source, V - target
const Vector2 ObjectHandler::getVectorDiff(const Vector2& u, const Vector2& v)
{
    return {v.x - u.x, v.y - u.y};
}

const Vector2 ObjectHandler::getVectorWithLength(const Vector2& u, double lngth)
{
    double dst = ObjectHandler::measureDistance({0, 0}, u);
    return {u.x / (float)dst * (float)lngth, u.y / (float)dst * (float)lngth};
}

Color enumHandler::tintToColor(const Tint& tnt)
{
    switch (tnt)
    {
        case DEFAULT_TINT:
            return {255,255,255,255};
        case RED_TINT:
            return {255, 64, 64, 255};
        case YELLOW_TINT:
            return {255, 255, 64, 255};
        case DARK_ORANGE_TINT:
            return {128, 64, 0, 255};   
        case GREEN_TINT:
            return {64, 255, 64, 255};
        case BLUE_TINT:
            return {64, 64, 255, 255};
        case VIOLET_TINT:
            return {168, 0, 255, 255};
        case DARK_PURPLE_TINT:
            return {48, 0, 128, 255};
        case LIGHTRED_TINT:
            return {255, 128, 128, 255};
        default:
            return {255, 255, 255, 255};
    };
}