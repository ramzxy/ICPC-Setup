// diameter = longest path in a tree (number of edges) with its
// two ends a and b. Needs bfs() from the graphs chapter.
int diameter(const vector<vi>& adj, int& a, int& b) {
    vi d = bfs(adj, 0);
    a = max_element(all(d)) - d.begin();   // farthest from 0
    d = bfs(adj, a);
    b = max_element(all(d)) - d.begin();   // farthest from a
    return d[b];
}
