int n, m;
cin >> n >> m;
Graph adj(n);
rep(i, 0, m) {
    int u, v; ll w;
    cin >> u >> v >> w;
    u--, v--;                  // input is 1..n, we use 0..n-1
    adj[u].push_back({v, w});
    adj[v].push_back({u, w});  // delete if edges are one-way
}
// unweighted: vector<vi> adj(n);  adj[u].push_back(v);
