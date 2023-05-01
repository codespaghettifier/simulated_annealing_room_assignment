#pragma once

#include "CostMatrix.hpp"

class RoomsAssignment
{
    using Room = std::pair<unsigned, unsigned>;

public:
    RoomsAssignment(unsigned numRooms);
    RoomsAssignment(unsigned numRooms, std::unique_ptr<CostMatrix> costMatrix, bool calculateNaive = true);

    void calculateNaiveSolution();
    inline unsigned getAssignment(unsigned person) const;
    int getSwapCost(unsigned roomA, bool personAFirst, unsigned roomB, bool personBFirst) const;
    void swap(unsigned roomA, bool personAFirst, unsigned roomB, bool personBFirst);
    void print(std::ostream& stream) const;

private:
    unsigned numRooms;
    std::unique_ptr<Room[]> assignments;
    std::unique_ptr<CostMatrix> costMatrix;
    int cost;

    void calculateCost();
};

std::ostream& operator<<(std::ostream& stream, const RoomsAssignment& costMatrix);

unsigned RoomsAssignment::getAssignment(unsigned person) const
{
    for(unsigned i = 0; i < numRooms; i++)
    {
        if(assignments[i].first == person || assignments[i].second == person) return i;
    }

    throw std::out_of_range("Person not assigned to any room.");
}
