// shortest walk that starts at 0 and visits all n nodes (n <= 18)
// d[u][v] = distance u -> v. dp[mask][v] = shortest walk from 0
// that visited exactly the nodes in mask and ends at v
ll tsp(const vector<vector<ll>>& d) {
    int n = d.size();
    vector<vector<ll>> dp(1 << n, vector<ll>(n, INF));
    dp[1][0] = 0;  // only node 0 visited, standing at 0
    rep(mask, 1, 1 << n) rep(v, 0, n) {
        if (dp[mask][v] == INF) continue;
        rep(u, 0, n) if (!(mask >> u & 1)) {  // u not visited yet
            int nm = mask | 1 << u;
            dp[nm][u] = min(dp[nm][u], dp[mask][v] + d[v][u]);
        }
    }
    ll best = INF;
    rep(v, 0, n) best = min(best, dp[(1 << n) - 1][v]);
    return best;  // round trip: dp[(1 << n) - 1][v] + d[v][0]
}
