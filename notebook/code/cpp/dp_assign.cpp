// n workers, n jobs, c[i][j] = cost of worker i doing job j.
// dp[mask] = cheapest way to give the jobs in mask to the first
// popcount(mask) workers. O(2^n * n)
ll assign(const vector<vector<ll>>& c) {
    int n = c.size();
    vector<ll> dp(1 << n, INF);
    dp[0] = 0;
    rep(mask, 0, 1 << n) {
        int i = __builtin_popcount(mask);  // the next worker
        rep(j, 0, n) if (!(mask >> j & 1)) {  // job j is still free
            int nm = mask | 1 << j;
            dp[nm] = min(dp[nm], dp[mask] + c[i][j]);
        }
    }
    return dp[(1 << n) - 1];
}
