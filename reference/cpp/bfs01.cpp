// all weights are 0 or 1. dist[v] = INF if unreachable
vector<ll> bfs01(const Graph& adj, int s) {
    vector<ll> dist(adj.size(), INF);
    deque<int> dq;
    dist[s] = 0; dq.push_back(s);
    while (!dq.empty()) {
        int u = dq.front(); dq.pop_front();
        for (auto [v, w] : adj[u]) if (dist[u] + w < dist[v]) {
            dist[v] = dist[u] + w;
            if (w == 0) dq.push_front(v);  // free edge: v first
            else dq.push_back(v);
        }
    }
    return dist;
}
