// read: vector<string> g(R); for (auto& row : g) cin >> row;
// rotate 90 degrees clockwise: an R x C grid becomes C x R
vector<string> rotateCW(const vector<string>& g) {
    int R = g.size(), C = g[0].size();
    vector<string> h(C, string(R, ' '));
    rep(r, 0, R) rep(c, 0, C) h[c][R - 1 - r] = g[r][c];
    return h;
}
// swap rows and columns: h[c][r] = g[r][c]
vector<string> transpose(const vector<string>& g) {
    int R = g.size(), C = g[0].size();
    vector<string> h(C, string(R, ' '));
    rep(r, 0, R) rep(c, 0, C) h[c][r] = g[r][c];
    return h;
}
// mirror left-right: for (auto& row : g) reverse(all(row));
// upside down: reverse(all(g));   counter-clockwise: rotateCW 3x
// 8 neighbours (with the diagonals):
int dr8[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dc8[] = {-1, 0, 1, -1, 1, -1, 0, 1};
// wrap around the edges (torus): nr = (r + dr8[d] + R) % R
