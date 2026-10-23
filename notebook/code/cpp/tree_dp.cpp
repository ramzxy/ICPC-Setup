// example tree DP: choose nodes, no two neighbours both chosen,
// maximise the total weight w. take[v] / skip[v] = best total
// inside the subtree of v when v is / is not chosen.
ll maxIndependentSet(const vector<vi>& adj, const vector<ll>& w) {
    int n = adj.size();
    vi par, dep;
    vi order = rootTree(adj, 0, par, dep);
    vector<ll> take(n), skip(n);
    for (int i = n - 1; i >= 0; i--) {   // children first
        int v = order[i];
        take[v] = w[v];
        skip[v] = 0;
        for (int c : adj[v]) if (c != par[v]) {   // c = a child
            take[v] += skip[c];
            skip[v] += max(take[c], skip[c]);
        }
    }
    return max(take[0], skip[0]);
}
