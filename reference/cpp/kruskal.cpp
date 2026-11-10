// minimum spanning tree. Needs DSU. edges = {w, u, v}.
// returns the total weight, or -1 if the graph is not connected
ll kruskal(int n, Edges edges) {
    sort(all(edges));  // tuples sort by their first field: w
    DSU dsu(n);
    ll total = 0;
    int taken = 0;
    for (auto [w, u, v] : edges)
        if (dsu.unite(u, v)) total += w, taken++;
    return taken == n - 1 ? total : -1;
}
