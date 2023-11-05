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