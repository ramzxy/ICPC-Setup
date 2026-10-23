// '#' = wall. dist[r][c] = steps from (sr, sc), -1 = unreachable
// directions: down, up, right, left
int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
vector<vi> gridBfs(const vector<string>& g, int sr, int sc) {
    int R = g.size(), C = g[0].size();
    vector<vi> dist(R, vi(C, -1));
    queue<pii> q;
    dist[sr][sc] = 0; q.push({sr, sc});
    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        rep(d, 0, 4) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
            if (g[nr][nc] == '#' || dist[nr][nc] != -1) continue;
            dist[nr][nc] = dist[r][c] + 1;
            q.push({nr, nc});
        }
    }
    return dist;
}
