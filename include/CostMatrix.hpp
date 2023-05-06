#pragma once

#include <memory>
#include <iostream>
//Required jsoncpp lib
//#include <jsoncpp/json/json.h>

// Person with index 0 represents an empty slot.
class CostMatrix
{
public:
    CostMatrix() = default;
    CostMatrix(const CostMatrix& other);

    //void loadFromJson(const Json::Value& peopleJson);
    void print(std::ostream& stream) const;
    inline unsigned getSize() const;
    inline int getCost(unsigned personA, unsigned personB) const;

    //temp function until jsoncpp works
    void loadFromTab(int **matrix);

private:
    unsigned size = 0;
    std::unique_ptr<int[]> cost;
};

std::ostream& operator<<(std::ostream& stream, const CostMatrix& costMatrix);

unsigned CostMatrix::getSize() const
{
    return size;
}

int CostMatrix::getCost(unsigned personA, unsigned personB) const
{
    return cost[personA * size + personB];
}
