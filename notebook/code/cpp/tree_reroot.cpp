// ans[v] = sum of the distances from v to all other nodes, for
// every v, in O(n). Needs rootTree.
vector<ll> sumOfDistances(const vector<vi>& adj) {
    int n = adj.size();
    vi par, dep;
    vi order = rootTree(adj, 0, par, dep);
    vector<ll> sz(n, 1), ans(n, 0);
    for (int i = n - 1; i > 0; i--)   // subtree sizes
        sz[par[order[i]]] += sz[order[i]];
    rep(v, 0, n) ans[0] += dep[v];   // the answer for the root
    rep(i, 1, n) {   // parents first: move the root down to v
        int v = order[i];
        ans[v] = ans[par[v]] - sz[v] + (n - sz[v]);
    }
    return ans;
}
