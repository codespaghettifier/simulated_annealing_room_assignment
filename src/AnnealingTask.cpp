#include "../include/AnnealingTask.hpp"

AnnealingTask::AnnealingTask(const AnnealingTask& other) :
    lowestCost(other.lowestCost),
    numRooms(other.numRooms),
    worseSolutionAcceptanceProbability(other.worseSolutionAcceptanceProbability),
    worseSolutionAcceptanceProbabilityRange(other.worseSolutionAcceptanceProbabilityRange),
    worseSolutionAcceptanceProbabilityDecreaseRate(other.worseSolutionAcceptanceProbabilityDecreaseRate),
    iterationsPerAcceptanceProbability(other.iterationsPerAcceptanceProbability),
    randomGeneratorSeed(other.randomGeneratorSeed)
{
    if(other.solution)
    {
        solution = std::make_unique<RoomsAssignment>(*other.solution);
    }
    if(other.costMatrix)
    {
        costMatrix = std::make_unique<CostMatrix>(*other.costMatrix);
    }
}

bool AnnealingTask::isRunnable() const
{
    bool runnable = true;
    runnable = runnable && (costMatrix || solution);
    runnable = runnable && !(costMatrix && numRooms * 2 < costMatrix->getSize() - 1);
    runnable = runnable && !(solution && numRooms != solution->getNumRooms() && !costMatrix);
    runnable = runnable && (worseSolutionAcceptanceProbability >= worseSolutionAcceptanceProbabilityRange.first && worseSolutionAcceptanceProbability <= worseSolutionAcceptanceProbabilityRange.second);
    return runnable;
}

AnnealingTask& AnnealingTask::operator=(AnnealingTask&& other)
{
    if(this == &other) return *this;

    lowestCost = std::move(other.lowestCost);
    numRooms = std::move(other.numRooms);
    solution = std::move(other.solution);
    costMatrix = std::move(other.costMatrix);
    worseSolutionAcceptanceProbability = std::move(other.worseSolutionAcceptanceProbability);
    worseSolutionAcceptanceProbabilityRange = std::move(other.worseSolutionAcceptanceProbabilityRange);
    worseSolutionAcceptanceProbabilityDecreaseRate = std::move(other.worseSolutionAcceptanceProbabilityDecreaseRate);
    iterationsPerAcceptanceProbability = std::move(other.iterationsPerAcceptanceProbability);
    randomGeneratorSeed = std::move(other.randomGeneratorSeed);

    return *this;
}