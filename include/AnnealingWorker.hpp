#pragma once

#include "AnnealingTask.hpp"
#include "Constants.hpp"

class AnnealingWorker
{
public:
    inline void updateLowestCost(int cost);
    void setTask(AnnealingTask&& task);
    void run();
    int getLowestCost() const;
    std::unique_ptr<RoomsAssignment> currentSolution;

private:
    
    void onLowerCostSolutionFound();
    void onFinish();

    AnnealingTask task;
    
};

void AnnealingWorker::updateLowestCost(int cost)
{
    task.lowestCost = cost;
}
