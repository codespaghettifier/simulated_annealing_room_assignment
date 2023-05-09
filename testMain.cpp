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

using namespace std;

int main(int argc, char *argv[])
{   
    
    std::ifstream file("MatrixCost.dat");
    if (!file.is_open()) {
        std::cerr << "Unable to open file.\n";
        return 1;
    }

    // load sizeOfCostMatrix
    int sizeOfCostMatrix;
    file >> sizeOfCostMatrix;
   

    //Matrix allocation
    int** matrix = new int* [sizeOfCostMatrix];
    for (int i = 0; i < sizeOfCostMatrix; i++) {
        matrix[i] = new int [sizeOfCostMatrix];
        for (int j = 0; j < sizeOfCostMatrix; j++) {
            file >> matrix[i][j];
        }
    }

    //---------MPI PART---------
    
    int myid, numprocs, source;

    
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Status status;
    

    //Create CostMatrix
    std::unique_ptr<CostMatrix> costMatrixToSend = std::make_unique<CostMatrix>();

    costMatrixToSend->loadFromTab(matrix, sizeOfCostMatrix);
    
    std::cout << *costMatrixToSend.get() <<std::endl;

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
    std::pair<int, unsigned> costSolutionTaskStatusMaster;
    std::pair<int, unsigned> costSolutionTaskStatusWorker;
    while(true)
    {
        if(myid == MASTER) 
        {

            MPI_Recv(&costSolutionTaskStatusMaster, sizeof(costSolutionTaskStatusMaster), MPI_BYTE, MPI_ANY_SOURCE, TAG, MPI_COMM_WORLD, &status);
            std::cout << "MASTER wait for messages" << std::endl;

            source = status.MPI_SOURCE;
            
            if(costSolutionTaskStatusMaster.second == FINAL_SOLUTION_FOUND)
            {
                std::cout << "MASTER Recieved FINAL_SOLUTION_FOUND form process: " << source << std::endl;
                activeNodes.erase(std::remove(activeNodes.begin(), activeNodes.end(), source), activeNodes.end());
                if (!activeNodes.empty())
                {
                    MPI_Bcast(&costSolutionTaskStatusMaster, sizeof(costSolutionTaskStatusMaster), MPI_BYTE, MASTER, MPI_COMM_WORLD);
                }
                else if( numberOfWorkers == 1)
                {
                    MPI_Bcast(&costSolutionTaskStatusMaster, sizeof(costSolutionTaskStatusMaster), MPI_BYTE, MASTER, MPI_COMM_WORLD);
                    break;
                }
                else 
                {
                    break;
                }
            }

            else if(costSolutionTaskStatusMaster.second == LOWER_COST_SOLUTION_FOUND && costSolutionTaskStatusMaster.first < bestSolution)
            {
                bestSolution = costSolutionTaskStatusMaster.first;
                std::cout << "MASTER Recieved LOWER_COST_SOLUTION_FOUND form process: " << source << std::endl;                
                if (!activeNodes.empty())
                {
                    MPI_Bcast(&costSolutionTaskStatusMaster, sizeof(costSolutionTaskStatusMaster), MPI_BYTE, MASTER, MPI_COMM_WORLD);
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


            std::cout<<"Process: "<<myid<<" Waiting for message"<<std::endl;
            MPI_Bcast(&costSolutionTaskStatusWorker, sizeof(costSolutionTaskStatusWorker), MPI_BYTE, MASTER, MPI_COMM_WORLD);

            if(costSolutionTaskStatusWorker.second == FINAL_SOLUTION_FOUND){
                std::cout << *worker.currentSolution.get() <<std::endl;
                break;
            }

            else if (costSolutionTaskStatusWorker.second == LOWER_COST_SOLUTION_FOUND)
            {
                
                if(worker.getLowestCost() > costSolutionTaskStatusWorker.first)
                {

                    std::cout << "Process: " << myid << " Update cost Recieved solution: " << costSolutionTaskStatusWorker.first << " Current Solution: " << worker.getLowestCost() << std::endl;
                    worker.updateLowestCost(costSolutionTaskStatusWorker.first);
                    
                }
                else
                {

                    std::cout << "Process: " << myid << " Already has better solution. Recieved solution: " << costSolutionTaskStatusWorker.first << " Current Solution: " << worker.getLowestCost() << std::endl;

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
