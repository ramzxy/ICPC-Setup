// split a into exactly k consecutive non-empty parts; minimise the
// total cost of the parts, here cost = (sum of the part)^2.
// dp[j][i] = best for the first i elements cut into j parts
ll splitK(const vector<ll>& a, int k) {
    int n = a.size();
    vector<ll> pre(n + 1, 0);
    rep(i, 0, n) pre[i + 1] = pre[i] + a[i];
    vector<vector<ll>> dp(k + 1, vector<ll>(n + 1, INF));
    dp[0][0] = 0;
    rep(j, 1, k + 1) rep(i, 1, n + 1)
        rep(p, 0, i) if (dp[j - 1][p] < INF) {  // last: a[p..i-1]
            ll s = pre[i] - pre[p];
            dp[j][i] = min(dp[j][i], dp[j - 1][p] + s * s);
        }
    return dp[k][n];  // INF if k > n
}
