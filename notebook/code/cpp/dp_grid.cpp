// paths from the top left to cell (r, c), moving only right or
// down and never onto '#'. g has R rows and C columns
const ll MOD = 1e9 + 7;
vector<vector<ll>> ways(R, vector<ll>(C, 0));
rep(r, 0, R) rep(c, 0, C) {
    if (g[r][c] == '#') continue;
    if (r == 0 && c == 0) { ways[r][c] = 1; continue; }
    if (r > 0) ways[r][c] += ways[r - 1][c];  // came from above
    if (c > 0) ways[r][c] += ways[r][c - 1];  // came from the left
    ways[r][c] %= MOD;
}
// answer: ways[R - 1][C - 1]
