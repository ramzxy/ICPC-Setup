// players take turns taking a[l] or a[r]; each wants the most for
// themselves. d[l][r] = mover's total - other's total on a[l..r]
vector<vector<ll>> d(n, vector<ll>(n, 0));
for (int l = n - 1; l >= 0; l--) {
    d[l][l] = a[l];
    rep(r, l + 1, n)
        d[l][r] = max(a[l] - d[l + 1][r], a[r] - d[l][r - 1]);
}
// first player gets (sum of a + d[0][n - 1]) / 2
