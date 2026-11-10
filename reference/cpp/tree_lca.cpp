// LCA = lowest common ancestor (deepest node above both a and b).
// up[j][v] = ancestor 2^j steps above v (the root above the root).
// Needs rootTree. Setup O(n log n), every query O(log n).
struct LCA {
    int LOG = 1;
    vector<vi> up;
    vi par, dep;
    LCA(const vector<vi>& adj, int root) {
        int n = adj.size();
        while ((1 << LOG) < n) LOG++;
        rootTree(adj, root, par, dep);
        up.assign(LOG, vi(n));
        rep(v, 0, n) up[0][v] = par[v] == -1 ? root : par[v];
        rep(j, 1, LOG) rep(v, 0, n)
            up[j][v] = up[j - 1][up[j - 1][v]];
    }
    int kth(int v, int k) {   // k steps up from v, -1 if too far
        if (k > dep[v]) return -1;
        rep(j, 0, LOG) if (k >> j & 1) v = up[j][v];
        return v;
    }
    int lca(int a, int b) {
        if (dep[a] < dep[b]) swap(a, b);
        a = kth(a, dep[a] - dep[b]);   // now both equally deep
        if (a == b) return a;
        // big jumps first; jump only if we stay below the LCA
        for (int j = LOG - 1; j >= 0; j--)
            if (up[j][a] != up[j][b]) a = up[j][a], b = up[j][b];
        return up[0][a];
    }
    int dist(int a, int b) {   // number of edges between a and b
        return dep[a] + dep[b] - 2 * dep[lca(a, b)];
    }
};
