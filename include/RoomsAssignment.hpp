#pragma once

#include "CostMatrix.hpp"

class RoomsAssignment
{
    using Room = std::pair<unsigned, unsigned>;

public:
    RoomsAssignment(unsigned numRooms);
    RoomsAssignment(unsigned numRooms, std::unique_ptr<CostMatrix> costMatrix, bool calculateNaive = true);

    void calculateNaiveSolution();
    unsigned getAssignment(unsigned personId) const;
    void print(std::ostream& stream) const;


private:
    unsigned numRooms;
    std::unique_ptr<Room[]> assignments;
    std::unique_ptr<CostMatrix> costMatrix;
    int cost;

    void calculateCost();
};

std::ostream& operator<<(std::ostream& stream, const RoomsAssignment& costMatrix);
