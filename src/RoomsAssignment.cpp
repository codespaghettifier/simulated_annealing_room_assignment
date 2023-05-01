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

int RoomsAssignment::getSwapCost(unsigned roomA, bool personAFirst, unsigned roomB, bool personBFirst) const
{
    const int costBefore = costMatrix->getCost(assignments[roomA].first, assignments[roomA].second) + costMatrix->getCost(assignments[roomB].first, assignments[roomB].second);
    const unsigned newRoomAFirst = personAFirst ? (personBFirst ? assignments[roomB].first : assignments[roomB].second) : assignments[roomA].first;
    const unsigned newRoomASecond = personAFirst ? assignments[roomA].second : (personBFirst ? assignments[roomB].first : assignments[roomB].second);
    const unsigned newRoomBFirst = personBFirst ? (personAFirst ? assignments[roomA].first : assignments[roomA].second) : assignments[roomB].first;
    const unsigned newRoomBSecond = personBFirst ? assignments[roomB].second : (personAFirst ? assignments[roomA].first : assignments[roomA].second);
    const int costAfter = costMatrix->getCost(newRoomAFirst, newRoomASecond) + costMatrix->getCost(newRoomBFirst, newRoomBSecond);
    return costAfter - costBefore;
}

void RoomsAssignment::swap(unsigned roomA, bool personAFirst, unsigned roomB, bool personBFirst)
{
    const int costBefore = costMatrix->getCost(assignments[roomA].first, assignments[roomA].second) + costMatrix->getCost(assignments[roomB].first, assignments[roomB].second);
    
    const unsigned newRoomAFirst = personAFirst ? (personBFirst ? assignments[roomB].first : assignments[roomB].second) : assignments[roomA].first;
    const unsigned newRoomASecond = personAFirst ? assignments[roomA].second : (personBFirst ? assignments[roomB].first : assignments[roomB].second);
    const unsigned newRoomBFirst = personBFirst ? (personAFirst ? assignments[roomA].first : assignments[roomA].second) : assignments[roomB].first;
    const unsigned newRoomBSecond = personBFirst ? assignments[roomB].second : (personAFirst ? assignments[roomA].first : assignments[roomA].second);
    assignments[roomA].first = newRoomAFirst;
    assignments[roomA].second = newRoomASecond;
    assignments[roomB].first = newRoomBFirst;
    assignments[roomB].second = newRoomBSecond;

    const int costAfter = costMatrix->getCost(assignments[roomA].first, assignments[roomA].second) + costMatrix->getCost(assignments[roomB].first, assignments[roomB].second);
    cost += costAfter - costBefore;
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