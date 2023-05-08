#include "include/Constants.hpp"
#include "include/CostMatrix.hpp"
#include "include/RoomsAssignment.hpp"
#include "include/AnnealingWorker.hpp"

#include <iostream>
#include <fstream>
#include <cstring>

#include <vector>
#include <algorithm>

#include <mpi.h>
//#include <jsoncpp/json/json.h>

using namespace std;

int main(int argc, char *argv[])
{   /*
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
    */
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


    // temporary CostMatrix initialization
    int test[11][11] = {{0, 79, 50, 52, 31, 76, 44, 88, 18, 89, 95},
                        {79, 0, 98, 37, 29, 46, 19, 23, 46, 20, 29},
                        {50, 98, 0, 16, 47, 99, 25, 28, 93, 1, 83},
                        {52, 37, 16, 0, 1, 65, 77, 56, 73, 86, 36},
                        {31, 29, 47, 1, 0, 78, 33, 95, 34, 55, 27},
                        {76, 46, 99, 65, 78, 0, 9, 62, 98, 65, 17},
                        {44, 19, 25, 77, 33, 9, 0, 51, 78, 45, 37},
                        {88, 23, 28, 56, 95, 62, 51, 0, 79, 34, 25},
                        {18, 46, 93, 73, 34, 98, 78, 79, 0, 92, 28},
                        {89, 20, 1, 86, 55, 65, 45, 34, 92, 0, 56},
                        {95, 29, 83, 36, 27, 17, 37, 25, 28, 56, 0}};
    unsigned size = sizeof(test) / sizeof(test[0]);

    int** root = new int* [size];
    int** root2 = new int* [size];
    for (unsigned i = 0; i < size; i++) {
        root[i] = new int[size];
        root2[i] = new int[size];
        for (unsigned j = 0; j < size; j++) {
            root[i][j] = test[i][j];
            root2[i][j] = test[i][j];
        }
    }

    /*
    std::unique_ptr<CostMatrix> costMatrix = std::unique_ptr<CostMatrix>(new CostMatrix);
    costMatrix->loadFromTab(root);
    //std::cout << *costMatrix.get() << std::endl << std::endl;

    std::pair<std::unique_ptr<char[]>, unsigned> serializedCostMatrix;
    {
        std::unique_ptr<CostMatrix> costMatrix2 = std::make_unique<CostMatrix>();
        costMatrix2->loadFromTab(root);
        std::cout << *costMatrix2.get() << std::endl << std::endl;

        serializedCostMatrix = costMatrix2->serialize();
    }
    std::cout<< "serialized data size: " << serializedCostMatrix.second << std::endl << std::endl;
    {
        std::unique_ptr<CostMatrix> costMatrix2 = std::make_unique<CostMatrix>();
        costMatrix2->deserialize(serializedCostMatrix.first.get());
        std::cout << "CostMatrix after serialization and deserialization:" << std::endl;
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

    std::pair<std::unique_ptr<char[]>, unsigned> serializedRoomsAssignment;
    {
        std::unique_ptr<CostMatrix> costMatrix2 = std::make_unique<CostMatrix>();
        costMatrix2->loadFromTab(root);

        std::unique_ptr<RoomsAssignment> roomsAssignment2 = std::make_unique<RoomsAssignment>(7, std::move(costMatrix2));
        std::cout << "roomsAssignment2:" << std::endl;
        std::cout << *roomsAssignment2.get() << std::endl << std::endl;

        serializedRoomsAssignment = roomsAssignment2->serialize();
    }
    std::cout<< "serialized data size: " << serializedRoomsAssignment.second << std::endl << std::endl;
    {
        std::unique_ptr<RoomsAssignment> roomsAssignment2 = std::make_unique<RoomsAssignment>();
        roomsAssignment2->deserialize(serializedRoomsAssignment.first.get());
        std::cout << "roomsAssignment2 after serialization and deserialization:" << std::endl;
        std::cout << *roomsAssignment2.get() << std::endl << std::endl;
    }

    AnnealingTask task;
    task.numRooms = 7;
    task.costMatrix = std::move(costMatrix);
    task.worseSolutionAcceptanceProbability = 0.9;
    task.worseSolutionAcceptanceProbabilityRange = std::pair<float, float>(0.0, 1.0);
    task.worseSolutionAcceptanceProbabilityDecreaseRate = 0.01;
    task.iterationsPerAcceptanceProbability = 100;
    task.randomGeneratorSeed = 42;

    std::pair<std::unique_ptr<char[]>, unsigned> serializedTask;
    {
        std::unique_ptr<CostMatrix> costMatrix2 = std::make_unique<CostMatrix>();
        costMatrix2->loadFromTab(root);

        //std::unique_ptr<RoomsAssignment> roomsAssignment2 = std::make_unique<RoomsAssignment>(7, std::move(costMatrix2));
    
        std::unique_ptr<AnnealingTask> task2 = std::make_unique<AnnealingTask>();
        task2.get()->numRooms = 7;
        task2.get()->costMatrix = std::move(costMatrix2);
        task2.get()->worseSolutionAcceptanceProbability = 0.9;
        task2.get()->worseSolutionAcceptanceProbabilityRange = std::pair<float, float>(0.0, 1.0);
        task2.get()->worseSolutionAcceptanceProbabilityDecreaseRate = 0.01;
        task2.get()->iterationsPerAcceptanceProbability = 100;
        task2.get()->randomGeneratorSeed = 42;
        std::cout << "task2:" << std::endl;
        std::cout << *task2.get() << std::endl << std::endl;

        serializedTask = task2->serialize();
    }
    std::cout<< "serialized data size: " << serializedTask.second << std::endl << std::endl;
    {
        std::unique_ptr<AnnealingTask> task2 = std::make_unique<AnnealingTask>();
        task2->deserialize(serializedTask.first.get());
        std::cout << "task2 after serialization and deserialization:" << std::endl;
        std::cout << *task2.get() << std::endl << std::endl;
    }
    
    AnnealingWorker worker;
    worker.setTask(std::move(task));
    worker.run();
    */

    //---------MPI PART---------
    
    int myid, numprocs, source;

    
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Status status;
    

    //Create CostMatrix
    std::unique_ptr<CostMatrix> costMatrixToSend = std::make_unique<CostMatrix>();
    costMatrixToSend->loadFromTab(root);

    int numberOfWorkers = numprocs - 1;

    std::vector<int> activeNodes;
    for (int i = 1; i <= numberOfWorkers; i++) {
        activeNodes.push_back(i);
    }

    //Create task
    AnnealingTask taskToSend;
    taskToSend.numRooms = 7;
    taskToSend.costMatrix = std::move(costMatrixToSend);
    taskToSend.worseSolutionAcceptanceProbability = 0.9;
    taskToSend.worseSolutionAcceptanceProbabilityRange = std::pair<float, float>(0.0, 1.0);
    taskToSend.worseSolutionAcceptanceProbabilityDecreaseRate = 0.01;
    taskToSend.iterationsPerAcceptanceProbability = 100;
    taskToSend.randomGeneratorSeed = 42;

    AnnealingWorker worker;

    //Serialize task in order to send it to nodes
    std::pair<std::unique_ptr<char[]>, unsigned> serializedTask;
    serializedTask = taskToSend.serialize();

    if(myid == MASTER){
        //send task to workers
        for(int i = 1; i <= numberOfWorkers; i++){
            MPI_Send(serializedTask.first.get(), serializedTask.second, MPI_CHAR, i, TAG, MPI_COMM_WORLD);
        } 
    }
    int loop = 0;
    int bestSolution = std::numeric_limits<int>::max(); 
    while(true)
    {
        if(myid == MASTER) 
        {
            std::pair<int, unsigned> costSolutionTaskStatus;
            MPI_Recv(&costSolutionTaskStatus, sizeof(costSolutionTaskStatus), MPI_BYTE, MPI_ANY_SOURCE, TAG, MPI_COMM_WORLD, &status);
            std::cout << "MASTER wait for messages" << std::endl;

            source = status.MPI_SOURCE;
            
            if(costSolutionTaskStatus.second == FINAL_SOLUTION_FOUND)
            {
                std::cout << "MASTER Recieved FINAL_SOLUTION_FOUND form process: " << source << std::endl;
                activeNodes.erase(std::remove(activeNodes.begin(), activeNodes.end(), source), activeNodes.end());
                if (!activeNodes.empty())
                {
                    MPI_Bcast(&costSolutionTaskStatus, sizeof(costSolutionTaskStatus), MPI_BYTE, MASTER, MPI_COMM_WORLD);
                }
                else 
                {
                    break;
                }
            }

            else if(costSolutionTaskStatus.second == LOWER_COST_SOLUTION_FOUND && costSolutionTaskStatus.first < bestSolution)
            {
                bestSolution = costSolutionTaskStatus.first;
                std::cout << "MASTER Recieved LOWER_COST_SOLUTION_FOUND form process: " << source << std::endl;                
                if (!activeNodes.empty())
                {
                    MPI_Bcast(&costSolutionTaskStatus, sizeof(costSolutionTaskStatus), MPI_BYTE, MASTER, MPI_COMM_WORLD);
                }
                else 
                {
                    break;
                }
                
            }

        }
        
        

        else
        {
            if (loop == 0)
            {
                std::unique_ptr<AnnealingTask> taskReceive = std::make_unique<AnnealingTask>();
                char buffer[1024];

                //receive task
                MPI_Recv(buffer, 1024, MPI_CHAR, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                std::cout << "Process with rank " << myid << " received the task." << std::endl;

                //deserialize
                taskReceive->deserialize(buffer);

                //Create Worker and set task
                worker.setTask(std::move(*taskReceive.get()));
                worker.run();
            }

            std::pair<int, unsigned> costSolutionTaskStatus;
            std::cout<<"Process: "<<myid<<" Waiting for message"<<std::endl;
            MPI_Bcast(&costSolutionTaskStatus, sizeof(costSolutionTaskStatus), MPI_BYTE, MASTER, MPI_COMM_WORLD);

            if(costSolutionTaskStatus.second == FINAL_SOLUTION_FOUND){
                std::cout << *worker.currentSolution.get() <<std::endl;
                break;
            }

            else if (costSolutionTaskStatus.second == LOWER_COST_SOLUTION_FOUND)
            {
                
                if(worker.getLowestCost() > costSolutionTaskStatus.first)
                {

                    std::cout << "Process: " << myid << " Update cost Recieved solution: " << costSolutionTaskStatus.first << " Current Solution: " << worker.getLowestCost() << std::endl;
                    worker.updateLowestCost(costSolutionTaskStatus.first);
                    
                }
                else
                {

                    std::cout << "Process: " << myid << " Already has better solution. Recieved solution: " << costSolutionTaskStatus.first << " Current Solution: " << worker.getLowestCost() << std::endl;

                }
                
                worker.run();
                
            }
        }
        loop ++;
    }//end of while
    std::cout<<"Process: " << myid << " Finished Job!" <<std::endl;
    MPI_Finalize();
    return 0;
}
