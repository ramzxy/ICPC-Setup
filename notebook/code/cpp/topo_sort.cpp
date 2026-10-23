// directed graph. Every edge u -> v has u before v in the result.
// result.size() < n  <=>  the graph has a cycle
vi topoSort(const vector<vi>& adj) {
    int n = adj.size();
    vi indeg(n, 0), order;
    rep(u, 0, n) for (int v : adj[u]) indeg[v]++;
    queue<int> q;
    rep(u, 0, n) if (indeg[u] == 0) q.push(u);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : adj[u]) if (--indeg[v] == 0) q.push(v);
    }
    return order;
}
