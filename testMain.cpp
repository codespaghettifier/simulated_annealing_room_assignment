#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>

#include "include/Constants.hpp"
#include "include/CostMatrix.hpp"

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

    CostMatrix costMatrix;
    costMatrix.loadFromJson(root);
    std::cout << costMatrix << std::endl;

    // Close the file and exit
    file.close();
    return 0;
}