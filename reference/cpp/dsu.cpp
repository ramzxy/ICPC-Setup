// Union-Find: find(x) = id of x's group, unite merges 2 groups.
// Both take almost O(1).
struct DSU {
    vi p, sz;
    DSU(int n) : p(n), sz(n, 1) { iota(all(p), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {  // false if already together
        a = find(a), b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a;
        sz[a] += sz[b];
        return true;
    }
};
// same group: d.find(a) == d.find(b)
// size of a's group: d.sz[d.find(a)]
