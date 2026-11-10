// jobs {e, s, p}: run from s to e (s < e) and earn p. Most profit
// with no two jobs overlapping (touching is ok). O(n log n).
// dp[i] = most profit using only the first i jobs (by end time)
ll bestProfit(vector<array<ll, 3>> jobs) {
    sort(all(jobs));
    int n = jobs.size();
    vector<ll> ends(n), dp(n + 1, 0);
    rep(i, 0, n) ends[i] = jobs[i][0];
    rep(i, 0, n) {
        auto [e, s, p] = jobs[i];
        // jobs 0..j-1 end at or before s, so they fit before job i
        int j = upper_bound(all(ends), s) - ends.begin();
        dp[i + 1] = max(dp[i], dp[j] + p);  // skip i, or take it
    }
    return dp[n];
}
