#include "../include/CostMatrix.hpp"

#include <cstring>

CostMatrix::CostMatrix(const CostMatrix& other)
: size(other.size)
{
    cost = std::make_unique<int[]>(size * size);
    std::copy(other.cost.get(), other.cost.get() + size * size, cost.get());
}

void CostMatrix::loadFromJson(const Json::Value& peopleJson)
{
    // The first person is not a real person. It's used to mark an empty slot.
    size = peopleJson.size() + 1;
    cost = std::unique_ptr<int[]>(new int[size * size]);

    cost[0] = 0;
    for(unsigned i = 1; i < size; i++)
    {
        cost[i] = peopleJson[i - 1]["cost"][0].asInt();
    }
    
    for(unsigned i = 1; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            cost[i * size + j] = peopleJson[i - 1]["cost"][j].asInt();
        }
    }
}

void CostMatrix::print(std::ostream& stream) const
{
    stream << "[";
    for(unsigned i = 0; i < size; i++)
    {
        if(i != 0) stream << ",\n";
        stream << "[";
        for(unsigned j = 0; j < size; j++)
        {
            if(j != 0) stream << ", ";
            stream << cost[i * size + j];
        }
        stream << "]";
    }
    stream << "]";
}

std::pair<std::unique_ptr<char[]>, unsigned> CostMatrix::serialize()
{
    unsigned dataSize = size * size * sizeof(int) + sizeof(unsigned);
    std::unique_ptr<char[]> data = std::make_unique<char[]>(dataSize);

    memcpy(data.get(), &size, sizeof(unsigned));

    char* start = data.get() + sizeof(unsigned);
    for(unsigned i = 0; i < size * size; i++)
    {
        memcpy(start, &cost[i], sizeof(cost[i]));
        start += sizeof(cost[i]);
    }

    return std::make_pair(std::move(data), dataSize);
}

void CostMatrix::deserialize(const char* data)
{
    std::memcpy(&size, data, sizeof(unsigned));
    cost = std::make_unique<int[]>(size * size);

    const char* start = data + sizeof(unsigned);
    for (unsigned i = 0; i < size * size; i++)
    {
        std::memcpy(&cost[i], start, sizeof(cost[i]));
        start += sizeof(cost[i]);
    }
}


std::ostream& operator<<(std::ostream& stream, const CostMatrix& costMatrix)
{
    costMatrix.print(stream);
    return stream;
}