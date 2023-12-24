#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int minCoins(const vector<int>& coinCounts, int target) 
{
    vector<int> dp(target + 1, INT_MAX);
    dp[0] = 0;

    vector<int> coins = { 1, 5, 10, 25 };

    for (int coin : coins)
    {
        for (int i = coin; i <= target; ++i)
        {
            if (dp[i - coin] != INT_MAX) {
                dp[i] = min(dp[i], dp[i - coin] + 1);
            }
        }
    }

    return (dp[target] == INT_MAX) ? -1 : dp[target];
}