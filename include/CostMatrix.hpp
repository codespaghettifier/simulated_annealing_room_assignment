#pragma once

#include <memory>
#include <iostream>

// Person with index 0 represents an empty slot.
class CostMatrix
{
public:
    CostMatrix() = default;
    CostMatrix(const CostMatrix& other);

    void print(std::ostream& stream) const;
    inline unsigned getSize() const;
    inline int getCost(unsigned personA, unsigned personB) const;
    std::pair<std::unique_ptr<char[]>, unsigned> serialize() const;
    unsigned deserialize(const char* data);

    void loadFromTab(int **matrix, int size);

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
