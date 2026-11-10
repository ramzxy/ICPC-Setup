// probability DP: dp[j] = P(being in state j after the steps so
// far); each step pushes every state's probability forward.
// Example: n tries, each succeeds with probability p;
// at the end dp[j] = P(exactly j successes). O(n^2)
vector<double> dp(n + 1, 0);
dp[0] = 1;  // before any try: 0 successes, for sure
rep(i, 0, n) {
    vector<double> nd(n + 1, 0);  // the states after try i
    rep(j, 0, i + 1) {
        nd[j] += dp[j] * (1 - p);   // try i fails
        nd[j + 1] += dp[j] * p;     // try i succeeds
    }
    dp = nd;
}
