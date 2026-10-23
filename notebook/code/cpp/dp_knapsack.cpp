// 0/1 knapsack: items {w, v}, each used at most once, total weight
// <= W. dp[c] = most value with weight <= c. O(n * W)
ll knapsack(const vector<pair<int, ll>>& items, int W) {
    vector<ll> dp(W + 1, 0);
    for (auto [w, v] : items)
        for (int c = W; c >= w; c--)  // backwards: item used once
            dp[c] = max(dp[c], dp[c - w] + v);
    return dp[W];
}
