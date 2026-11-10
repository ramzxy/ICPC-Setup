// Euler path: a walk that uses every edge exactly once. Returns
// its m + 1 nodes, or {} if there is none. Edges {u, v} may
// repeat; set directed = true for one-way edges.
vi eulerPath(int n, const vector<pii>& edges, bool directed) {
    int m = edges.size();
    vector<vector<pii>> adj(n);   // adj[u] = {to, edge id}
    vi deg(n, 0);   // undirected: degree. directed: out - in
    rep(i, 0, m) {
        auto [u, v] = edges[i];
        adj[u].push_back({v, i});
        if (directed) deg[u]++, deg[v]--;
        else adj[v].push_back({u, i}), deg[u]++, deg[v]++;
    }
    int start = m ? edges[0].first : 0, odd = 0;
    rep(v, 0, n) {
        if (directed && abs(deg[v]) > 1) return {};
        if (directed ? deg[v] != 0 : deg[v] % 2 != 0) odd++;
        if (directed ? deg[v] == 1 : deg[v] % 2 != 0) start = v;
    }
    if (odd > 2) return {};
    vector<bool> used(m, false);
    vi it(n, 0), st = {start}, path;
    while (!st.empty()) {   // Hierholzer: walk until stuck,
        int u = st.back();  // then back up and add detours
        while (it[u] < (int)adj[u].size()
               && used[adj[u][it[u]].second]) it[u]++;
        if (it[u] == (int)adj[u].size()) {
            path.push_back(u);
            st.pop_back();
        } else {
            auto [v, id] = adj[u][it[u]];
            used[id] = true;
            st.push_back(v);
        }
    }
    if ((int)path.size() != m + 1) return {};   // not connected
    reverse(all(path));
    return path;
}
