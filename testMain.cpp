#include "include/Constants.hpp"
#include "include/CostMatrix.hpp"
#include "include/RoomsAssignment.hpp"
#include "include/AnnealingWorker.hpp"

#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>

using namespace std;

int main()
{
    // Open the JSON file
    ifstream file(TEST_DATA_PATH);
    if (!file.is_open())
    {
        cerr << "Error opening file." << endl;
        return 1;
    }

    // Parse the JSON file
    Json::Value root;
    file >> root;

    // // Loop through the list of dictionaries and print out the data
    // for (const auto& person : root)
    // {
    //     cout << "Name: " << person["name"].asString() << endl;
    //     cout << "ID: " << person["id"].asInt() << endl;
    //     cout << "Costs: ";
    //     for (const auto& cost : person["cost"])
    //     {
    //         cout << cost.asInt() << " ";
    //     }
    //     cout << endl;
    // }

    std::unique_ptr<CostMatrix> costMatrix = std::unique_ptr<CostMatrix>(new CostMatrix);
    costMatrix->loadFromJson(root);
    std::cout << *costMatrix.get() << std::endl << std::endl;

    std::pair<std::unique_ptr<char[]>, unsigned> serializedCostMatrix;
    {
        std::unique_ptr<CostMatrix> costMatrix2 = std::unique_ptr<CostMatrix>(new CostMatrix);
        costMatrix2->loadFromJson(root);
        std::cout << *costMatrix2.get() << std::endl << std::endl;

        serializedCostMatrix = costMatrix2->serialize();
    }
    std::cout<< "serialized data size: " << serializedCostMatrix.second << std::endl << std::endl;
    {
        std::unique_ptr<CostMatrix> costMatrix2 = std::make_unique<CostMatrix>();
        costMatrix2->deserialize(serializedCostMatrix.first.get());
        std::cout << *costMatrix2.get() << std::endl << std::endl;
    }

    // RoomsAssignment roomsAssignment(7, std::move(costMatrix));
    // std::cout << roomsAssignment << std::endl;

    // std::cout << roomsAssignment.getSwapCost(0, true, 1, true) << std::endl;
    // roomsAssignment.swap(0, true, 1, true);
    // std::cout << roomsAssignment << std::endl;

    // std::cout << roomsAssignment.getSwapCost(0, true, 1, false) << std::endl;
    // roomsAssignment.swap(0, true, 1, false);
    // std::cout << roomsAssignment << std::endl;

    // std::cout << roomsAssignment.getSwapCost(0, false, 1, true) << std::endl;
    // roomsAssignment.swap(0, false, 1, true);
    // std::cout << roomsAssignment << std::endl;

    // std::cout << roomsAssignment.getSwapCost(0, false, 1, false) << std::endl;
    // roomsAssignment.swap(0, false, 1, false);
    // std::cout << roomsAssignment << std::endl;

    AnnealingTask task;
    task.numRooms = 7;
    task.costMatrix = std::move(costMatrix);
    task.worseSolutionAcceptanceProbability = 0.9;
    task.worseSolutionAcceptanceProbabilityRange = std::pair<float, float>(0.0, 1.0);
    task.worseSolutionAcceptanceProbabilityDecreaseRate = 0.01;
    task.iterationsPerAcceptanceProbability = 100;
    task.randomGeneratorSeed = 42;

    AnnealingWorker worker;
    worker.setTask(std::move(task));
    worker.run();

    // Close the file and exit
    file.close();
    return 0;
}