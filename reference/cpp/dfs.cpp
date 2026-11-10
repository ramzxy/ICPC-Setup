// connected components (undirected): comp[v] = id of v's group.
// Uses the GLOBAL adj: fill it in main, don't declare a new one.
vector<vi> adj;
vi comp;
void dfs(int u, int id) {
    comp[u] = id;
    for (int v : adj[u]) if (comp[v] == -1) dfs(v, id);
}
// usage: comp.assign(n, -1); int k = 0;  // k = #components
//        rep(u, 0, n) if (comp[u] == -1) dfs(u, k++);
