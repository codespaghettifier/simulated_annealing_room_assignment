#include "../include/RoomsAssignment.hpp"

RoomsAssignment::RoomsAssignment(unsigned numRooms)
: numRooms(numRooms)
{
    assignments = std::unique_ptr<Room[]>(new Room[numRooms]);
    for(unsigned i = 0; i < numRooms; i++)
    {
        assignments[i] = Room(0, 0);
    }
}

RoomsAssignment::RoomsAssignment(unsigned numRooms, std::unique_ptr<CostMatrix> costMatrix, bool calculateNaive)
: RoomsAssignment(numRooms)
{
    this->costMatrix = std::move(costMatrix);

    if(calculateNaive) calculateNaiveSolution();
}

void RoomsAssignment::calculateNaiveSolution()
{
    if(costMatrix.get() == nullptr) throw std::logic_error("Cost matrix not initlized.");

    if(numRooms * 2 < costMatrix->getSize() - 1) throw std::logic_error("Not enough rooms to fit all the people.");

    for(unsigned i = 0; i < numRooms; i++)
    {
        // Person with index 0 represents an empty slot.
        const unsigned firstPerson = i * 2 + 1;
        if(firstPerson >= costMatrix->getSize()) break;
        assignments[i].first = firstPerson;

        const unsigned secondPerson = i * 2 + 2;
        if(secondPerson >= costMatrix->getSize()) break;
        assignments[i].second = secondPerson;
    }
    calculateCost();
}

unsigned RoomsAssignment::getAssignment(unsigned personId) const
{
    for(unsigned i = 0; i < numRooms; i++)
    {
        if(assignments[i].first == personId || assignments[i].second == personId) return i;
    }

    throw std::out_of_range("Person not assigned to any room.");
}

void RoomsAssignment::print(std::ostream& stream) const
{
    stream << "{";

    stream << "rooms: ";
    stream << "[";
    for(unsigned i = 0; i < numRooms; i++)
    {
        if(i != 0) stream << ",\n";
        stream << "[";
        stream << assignments[i].first;
        stream << ", " << assignments[i].second;
        stream << "]";
    }
    stream << "]";

    stream << ", \ncost: " << cost;

    stream << "}";
}

void RoomsAssignment::calculateCost()
{
    cost = 0;
    for(unsigned i = 0; i < numRooms; i++)
    {
        cost += costMatrix->getCost(assignments[i].first, assignments[i].second);
    }
}

std::ostream& operator<<(std::ostream& stream, const RoomsAssignment& costMatrix)
{
    costMatrix.print(stream);
    return stream;
}