#include "../include/CostMatrix.hpp"

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

std::ostream& operator<<(std::ostream& stream, const CostMatrix& costMatrix)
{
    costMatrix.print(stream);
    return stream;
}