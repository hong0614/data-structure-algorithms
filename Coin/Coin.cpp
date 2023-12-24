#include <iostream>
#include <vector>
#include <climits>
#include <fstream>
#include <chrono>
#include <sstream>
#include"coin.h"
using namespace std;


int main() {
    ifstream inputFile("input.txt");
    ofstream outputFile("output.txt");

    if (!inputFile) 
    {
        cerr << "Error: file could not be opened" << endl;
        return -1;
    }

    vector<int> coinCounts(4);
    int target;

    while (inputFile >> coinCounts[0] >> coinCounts[1] >> coinCounts[2] >> coinCounts[3]) 
    {
        inputFile >> target;

        auto start = chrono::high_resolution_clock::now();
        int result = minCoins(coinCounts, target);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;

        if (result == -1) 
        {
            outputFile << "Solution does not exist." << endl;
        }
        else 
        {
            outputFile << "Minimum number of coins required: " << result << endl;
        }

        outputFile << "Elapsed time: " << elapsed.count() << " seconds" << endl;
    }

    inputFile.close();
    outputFile.close();
    return 0;
}
