// all weights >= 0. dist[v] = INF if unreachable.
// par[v] = node before v on a shortest path (-1: none)
vector<ll> dijkstra(const Graph& adj, int s, vi& par) {
    int n = adj.size();
    vector<ll> dist(n, INF);
    par.assign(n, -1);
    // min-heap of {distance, node}: smallest distance on top
    priority_queue<pair<ll, int>, vector<pair<ll, int>>,
                   greater<>> pq;
    dist[s] = 0; pq.push({0, s});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;  // outdated entry: skip it
        for (auto [v, w] : adj[u]) if (d + w < dist[v]) {
            dist[v] = d + w;
            par[v] = u;
            pq.push({dist[v], v});
        }
    }
    return dist;
}
// path s -> t (check dist[t] < INF first):
//   vi path;
//   for (int v = t; v != -1; v = par[v]) path.push_back(v);
//   reverse(all(path));
