#include "../include/AnnealingWorker.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cstring>

#include "mpi.h"

void AnnealingWorker::setTask(AnnealingTask&& task)
{
    this->task = std::move(task);
}

int AnnealingWorker::getLowestCost() const
{
    return task.lowestCost;
}

void AnnealingWorker::run()
{
    if(!task.isRunnable()) throw std::logic_error("AnnealingWorker cannot run a task.");

    if (!task.solution || task.numRooms != task.solution->getNumRooms())
    {
        task.solution = std::make_unique<RoomsAssignment>(task.numRooms, std::make_unique<CostMatrix>(*task.costMatrix));
        task.solution->calculateNaiveSolution();
    }

    if (!currentSolution) currentSolution = std::move(task.solution);

    std::srand(task.randomGeneratorSeed);

    while(task.worseSolutionAcceptanceProbability >= task.worseSolutionAcceptanceProbabilityRange.first)
    {
        long iteration = 0;
        while(iteration < task.iterationsPerAcceptanceProbability)
        {
            // Generate random swap
            // swapRoomA == swapRoomB is ok (has no effect)
            unsigned swapRoomA = std::rand() % task.numRooms;
            unsigned swapRoomB = std::rand() % task.numRooms;
            bool personAFirst = std::rand() % 2;
            bool personBFirst = std::rand() % 2;

            if(currentSolution->getSwapCost(swapRoomA, personAFirst, swapRoomB, personBFirst) <= 0 || static_cast<float>(std::rand()) / RAND_MAX <= task.worseSolutionAcceptanceProbability)
            {
                currentSolution->swap(swapRoomA, personAFirst, swapRoomB, personBFirst);
                if(currentSolution->getCost() < task.lowestCost) break;
            }

            iteration++;
        }
        if(iteration < task.iterationsPerAcceptanceProbability) break;  // Lower cost solution Found

        task.worseSolutionAcceptanceProbability -= task.worseSolutionAcceptanceProbabilityDecreaseRate;
    }

    if(task.worseSolutionAcceptanceProbability >= task.worseSolutionAcceptanceProbabilityRange.first)  // Lower cost solution Found
    {
        onLowerCostSolutionFound();
        // Return since a task modification or a new task is expected
        return;
    }

    if(task.worseSolutionAcceptanceProbability < task.worseSolutionAcceptanceProbabilityRange.first) onFinish();
}

void AnnealingWorker::onLowerCostSolutionFound()
{

    std::cout << "Lower cost solution found, cost: " << currentSolution->getCost() << std::endl;
   
    // Send message to the master here
    // In response, task should be modified (decreased lowestCost) for all workers or a new task shoud be set.
    // After that run should be called again.

    std::pair<int, const unsigned int> lowerSolutionFound = {currentSolution->getCost(), LOWER_COST_SOLUTION_FOUND};
    MPI_Send(&lowerSolutionFound, sizeof(lowerSolutionFound), MPI_BYTE, MASTER, TAG, MPI_COMM_WORLD);
}

void AnnealingWorker::onFinish()
{
    // Send message to the master here

    std::pair<int, const unsigned int> finalSolutionFound = {currentSolution->getCost(), FINAL_SOLUTION_FOUND};
    MPI_Send(&finalSolutionFound, sizeof(finalSolutionFound), MPI_BYTE, MASTER, TAG, MPI_COMM_WORLD);
    
    return;
}
