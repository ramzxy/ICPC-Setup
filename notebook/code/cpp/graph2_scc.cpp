// strongly connected components (Kosaraju) of a directed graph.
// scc[v] = component of v, k = number of components. Ids are in
// topological order: every edge u -> v has scc[u] <= scc[v].
struct SCC {
    int n, k = 0;
    vector<vi> g, rg;   // the edges, and the edges reversed
    vi order, scc;
    vector<bool> seen;
    SCC(const vector<vi>& adj)
        : n(adj.size()), g(adj), rg(n), scc(n, -1), seen(n) {
        rep(u, 0, n) for (int v : g[u]) rg[v].push_back(u);
        rep(u, 0, n) if (!seen[u]) dfs1(u);
        reverse(all(order));   // latest finished first
        for (int u : order) if (scc[u] == -1) dfs2(u, k++);
    }
    void dfs1(int u) {   // normal DFS, remember finishing order
        seen[u] = true;
        for (int v : g[u]) if (!seen[v]) dfs1(v);
        order.push_back(u);
    }
    void dfs2(int u, int id) {   // DFS on the reversed edges
        scc[u] = id;
        for (int v : rg[u]) if (scc[v] == -1) dfs2(v, id);
    }
    vector<vi> condense() {   // the DAG with one node per component
        vector<vi> dag(k);    // (an edge may appear more than once)
        rep(u, 0, n) for (int v : g[u])
            if (scc[u] != scc[v]) dag[scc[u]].push_back(scc[v]);
        return dag;
    }
};
