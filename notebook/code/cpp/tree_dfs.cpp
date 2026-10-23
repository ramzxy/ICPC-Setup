// DFS order without recursion, from root r. tin[v] = when v is
// entered. The subtree of v gets the times tin[v] .. tout[v]-1,
// so u is below v exactly when tin[v] <= tin[u] < tout[v].
void dfsOrder(const vector<vi>& adj, int r, vi& tin, vi& tout) {
    int n = adj.size(), timer = 0;
    tin.assign(n, -1);
    tout.assign(n, -1);
    vi it(n, 0), st = {r};   // it[u] = next neighbour of u to try
    tin[r] = timer++;
    while (!st.empty()) {
        int u = st.back();   // depth of u = st.size() - 1
        if (it[u] < (int)adj[u].size()) {
            int v = adj[u][it[u]++];
            if (tin[v] == -1) {   // enter v, a child of u
                tin[v] = timer++;
                st.push_back(v);
            }
        } else {   // leave u: its whole subtree is done
            tout[u] = timer;
            st.pop_back();
        }
    }
}
