// longest common subsequence of a and b, O(|a| * |b|).
// dp[i][j] = LCS length of the first i chars of a, j chars of b
string lcs(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    vector<vi> dp(n + 1, vi(m + 1, 0));
    rep(i, 0, n) rep(j, 0, m)
        dp[i + 1][j + 1] = a[i] == b[j] ? dp[i][j] + 1
                         : max(dp[i][j + 1], dp[i + 1][j]);
    string res;  // walk back from dp[n][m] to find one LCS
    for (int i = n, j = m; i > 0 && j > 0;) {
        if (a[i - 1] == b[j - 1]) res += a[i - 1], i--, j--;
        else if (dp[i - 1][j] >= dp[i][j - 1]) i--;
        else j--;
    }
    reverse(all(res));
    return res;  // length: dp[n][m]
}
