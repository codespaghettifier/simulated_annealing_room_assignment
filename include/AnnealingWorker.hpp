# pragma once

#include "AnnealingTask.hpp"

class AnnealingWorker
{
public:
    void setTask(AnnealingTask&& task);
    void run();

private:
    inline void updateLowestCost(int cost);
    void onLowerCostSolutionFound();
    void onFinish();

    AnnealingTask task;
    std::unique_ptr<RoomsAssignment> currentSolution;
};

void AnnealingWorker::updateLowestCost(int cost)
{
    task.lowestCost = cost;
}
