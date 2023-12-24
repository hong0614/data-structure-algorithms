#include <iostream>
#include <vector>
#include <climits>
#include <fstream>
#include <chrono>
#include <sstream>
using namespace std;

int minCoins(const vector<int>& availableCoins, int targetAmount)
{
    // Fixed denominations
    vector<int> denominations = { 1, 5, 10, 25 };
    int numCoinTypes = denominations.size();
    vector<vector<int>> dp(numCoinTypes + 1, vector<int>(targetAmount + 1, INT_MAX));

    // Initializing the first row and column of the DP table
    for (int i = 0; i <= targetAmount; i++)
    {
        dp[0][i] = INT_MAX;
    }
    for (int i = 0; i <= numCoinTypes; i++)
    {
        dp[i][0] = 0;
    }

    // Building up the DP table
    for (int i = 1; i <= numCoinTypes; i++)
    {
        int coinValue = denominations[i - 1];
        int coinLimit = availableCoins[i - 1];

        for (int j = 0; j <= targetAmount; j++)
        {
            dp[i][j] = dp[i - 1][j];

            for (int k = 1; k <= coinLimit && k * coinValue <= j; k++)
            {
                if (dp[i - 1][j - k * coinValue] != INT_MAX)
                {
                    dp[i][j] = min(dp[i][j], dp[i - 1][j - k * coinValue] + k);
                }
            }
        }
    }

    return dp[numCoinTypes][targetAmount] == INT_MAX ? -1 : dp[numCoinTypes][targetAmount];
}

int main()
{
    ifstream inputFile("input1.txt");
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
