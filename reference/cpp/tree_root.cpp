// root the tree at r: par[v] = parent (-1 for r), dep[v] = depth.
// Returns the BFS order: every parent comes before its children,
// so a loop over it BACKWARDS handles children before parents.
vi rootTree(const vector<vi>& adj, int r, vi& par, vi& dep) {
    int n = adj.size();
    par.assign(n, -1);
    dep.assign(n, 0);
    vi order = {r};
    for (int i = 0; i < (int)order.size(); i++) {   // it grows
        int u = order[i];
        for (int v : adj[u]) if (v != par[u]) {
            par[v] = u;
            dep[v] = dep[u] + 1;
            order.push_back(v);
        }
    }
    return order;
}
