// 2-colouring: col[v] = 0 or 1, the two ends of every edge differ.
// Returns false if that is impossible (there is an odd cycle).
bool bipartite(const vector<vi>& adj, vi& col) {
    int n = adj.size();
    col.assign(n, -1);
    rep(s, 0, n) if (col[s] == -1) {   // BFS in every component
        queue<int> q;
        col[s] = 0; q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (col[v] == -1) col[v] = 1 - col[u], q.push(v);
                else if (col[v] == col[u]) return false;
            }
        }
    }
    return true;
}
