#include "include/Constants.hpp"
#include "include/CostMatrix.hpp"
#include "include/RoomsAssignment.hpp"
#include "include/AnnealingWorker.hpp"

#include <iostream>
#include <fstream>
#include <cstring>

// commented until json issue is solved
//#include <jsoncpp/json/json.h>

#include <mpi.h>

using namespace std;

int main(int argc, char *argv[])
{
    
    // Open the JSON file
    /*  std::cout << TEST_DATA_PATH << std::endl;

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

    unsigned size = 11;
    int** root = new int* [size];
    for (unsigned i = 0; i < size; i++) {
        root[i] = new int[size];
        for (unsigned j = 0; j < size; j++) {
            root[i][j] = test[i][j];
        }
    }


    std::unique_ptr<CostMatrix> costMatrix = std::unique_ptr<CostMatrix>(new CostMatrix);
    costMatrix->loadFromTab(root);
    std::cout << *costMatrix.get() << std::endl << std::endl;
    /* Room Swap
    RoomsAssignment roomsAssignment(7, std::move(costMatrix));
    std::cout << roomsAssignment << std::endl;

    std::cout << roomsAssignment.getSwapCost(0, true, 1, true) << std::endl;
    roomsAssignment.swap(0, true, 1, true);
    std::cout << roomsAssignment << std::endl;

    std::cout << roomsAssignment.getSwapCost(0, true, 1, false) << std::endl;
    roomsAssignment.swap(0, true, 1, false);
    std::cout << roomsAssignment << std::endl;

    std::cout << roomsAssignment.getSwapCost(0, false, 1, true) << std::endl;
    roomsAssignment.swap(0, false, 1, true);
    std::cout << roomsAssignment << std::endl;

    std::cout << roomsAssignment.getSwapCost(0, false, 1, false) << std::endl;
    roomsAssignment.swap(0, false, 1, false);
    std::cout << roomsAssignment << std::endl;
    */
    /* Task + worker
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
    worker.run();*/
    cout<<endl<<"---------MPI PART---------"<<endl<<endl;
    
    int myid, numprocs;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    //Master Part

    if(myid == MASTER) 
    {
        int n = numprocs - 1;
        
        //int source;
        cout<<"master"<<endl;

        //Create task
        AnnealingTask task;
        task.numRooms = 7;
        //Unique_ptr issue with serialization
        //task.costMatrix = std::move(costMatrix);
        task.worseSolutionAcceptanceProbability = 0.9;
        task.worseSolutionAcceptanceProbabilityRange = std::pair<float, float>(0.0, 1.0);
        task.worseSolutionAcceptanceProbabilityDecreaseRate = 0.01;
        task.iterationsPerAcceptanceProbability = 100;
        task.randomGeneratorSeed = 42;

        //Serialize task in order to send it to nodes
        int bufferSize = sizeof(AnnealingTask);
        char buffer[bufferSize];
        memcpy(buffer, &task, bufferSize);

        
        
        
        /* check task data 
        AnnealingTask deserializedTask = *reinterpret_cast<AnnealingTask*>(buffer);
        std::cout << "Received task with values:" << std::endl;
        std::cout << "numRooms: " << deserializedTask.numRooms << std::endl;
        std::cout << "costMatrix: \n" << *deserializedTask.costMatrix.get() << std::endl;
        std::cout << "numRooms: " << deserializedTask.worseSolutionAcceptanceProbability << std::endl;
        std::cout << "worseSolutionAcceptanceProbabilityRange.first: " << deserializedTask.worseSolutionAcceptanceProbabilityRange.first << std::endl;
        std::cout << "worseSolutionAcceptanceProbabilityRange.second: " << deserializedTask.worseSolutionAcceptanceProbabilityRange.second << std::endl;
        std::cout << "worseSolutionAcceptanceProbabilityDecreaseRate: " << deserializedTask.worseSolutionAcceptanceProbabilityDecreaseRate << std::endl;
        std::cout << "iterationsPerAcceptanceProbability: " << deserializedTask.iterationsPerAcceptanceProbability << std::endl;
        std::cout << "randomGeneratorSeed: " << deserializedTask.randomGeneratorSeed << std::endl;
        */

        //send task to workers
        for(int i = 1; i <= n; i++){
            MPI_Send(buffer, bufferSize, MPI_CHAR, i, TAG, MPI_COMM_WORLD);
        }

        /*
        //TO DO
        //receive results from workers

        for(int i = 1; i <= n; i++){
            MPI_Recv(&buffer, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            source = status.MPI_SOURCE;
            cout<<"Recieved res: "<<buffer<<" Form process: "<<source<<endl;
            result += buffer;

        }
        */
        std::cout<<"\n\n\n\n";

    }
    //Worker parts
    else{
        cout<<"Worker"<<endl;

        // receive and deserialize AnnealingTask object from master
        int buffer_size = sizeof(AnnealingTask);
        char buffer[buffer_size];

        MPI_Recv(buffer, buffer_size, MPI_CHAR, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        //deserialize
        AnnealingTask task = *reinterpret_cast<AnnealingTask*>(buffer);
        std::cout << "task.numRooms " << task.numRooms << std::endl;

        //Create Worker and set task
        AnnealingWorker worker;
        worker.setTask(std::move(task));
        worker.run();
    }

    
    MPI_Finalize();
    
    
    //Json Part not working
    //Close the file and exit
    //file.close();

       for (int i = 0; i < 11; i++) {
        delete[] root[i];
    }
    delete[] root; 
    return 0;
}
