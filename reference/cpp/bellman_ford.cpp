// edges {w, u, v}: u -> v with weight w (w may be negative).
// returns false if a negative cycle is reachable from s
bool bellmanFord(int n, const Edges& edges, int s,
                 vector<ll>& dist) {
    dist.assign(n, INF);
    dist[s] = 0;
    rep(round, 0, n) {
        bool changed = false;
        for (auto [w, u, v] : edges)
            if (dist[u] < INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                changed = true;
            }
        if (!changed) return true;
    }
    return false;  // still improving after n rounds
}
