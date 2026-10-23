// fewest single-char inserts, deletes and replaces to turn a into
// b. dp[i][j] = cost for the first i chars of a -> first j of b
int editDistance(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    vector<vi> dp(n + 1, vi(m + 1));
    rep(i, 0, n + 1) dp[i][0] = i;  // delete all i chars
    rep(j, 0, m + 1) dp[0][j] = j;  // insert all j chars
    rep(i, 1, n + 1) rep(j, 1, m + 1)
        dp[i][j] = min({dp[i - 1][j] + 1,   // delete a[i-1]
                        dp[i][j - 1] + 1,   // insert b[j-1]
                        dp[i - 1][j - 1] + (a[i - 1] != b[j - 1])});
    return dp[n][m];
}
