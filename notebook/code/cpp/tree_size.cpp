// sz[v] = number of nodes in the subtree of v (v included).
// Needs order and par from rootTree.
vi sz(n, 1);
for (int i = n - 1; i > 0; i--) sz[par[order[i]]] += sz[order[i]];
