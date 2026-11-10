// n piles in a row; merging two neighbouring piles costs their
// total size. Cheapest way to merge everything. O(n^3).
// dp[l][r] = cheapest way to merge piles l..r into one
ll mergePiles(const vector<ll>& a) {
    int n = a.size();
    vector<ll> pre(n + 1, 0);
    rep(i, 0, n) pre[i + 1] = pre[i] + a[i];
    vector<vector<ll>> dp(n, vector<ll>(n, 0));  // dp[i][i] = 0
    for (int len = 2; len <= n; len++)  // short pieces first
        for (int l = 0, r = len - 1; r < n; l++, r++) {
            dp[l][r] = INF;
            rep(m, l, r)  // the last merge joins l..m and m+1..r
                dp[l][r] = min(dp[l][r], dp[l][m] + dp[m + 1][r]);
            dp[l][r] += pre[r + 1] - pre[l];  // its cost
        }
    return dp[0][n - 1];
}
