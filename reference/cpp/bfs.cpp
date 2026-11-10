// dist[v] = fewest edges from s to v, -1 if unreachable
vi bfs(const vector<vi>& adj, int s) {
    vi dist(adj.size(), -1);
    queue<int> q;
    dist[s] = 0; q.push(s);  // multi-source: same for each source
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) if (dist[v] == -1) {
            dist[v] = dist[u] + 1;
            q.push(v);
        }
    }
    return dist;
}
