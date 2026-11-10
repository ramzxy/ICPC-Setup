// P[i][j] = sum of g[0..i-1][0..j-1]  (g has R rows, C columns)
vector<vector<ll>> P(R + 1, vector<ll>(C + 1, 0));
rep(i, 0, R) rep(j, 0, C)
    P[i+1][j+1] = g[i][j] + P[i][j+1] + P[i+1][j] - P[i][j];
// sum of the rectangle rows r1..r2, columns c1..c2 (inclusive)
auto rect = [&](int r1, int c1, int r2, int c2) {
    return P[r2+1][c2+1] - P[r1][c2+1] - P[r2+1][c1] + P[r1][c1];
};
