// cycles of a permutation p of 0..n-1, where i goes to p[i]
vector<vi> cycles(const vi& p) {
    int n = p.size();
    vector<bool> seen(n, false);
    vector<vi> cyc;
    rep(i, 0, n) if (!seen[i]) {
        cyc.push_back({});
        for (int j = i; !seen[j]; j = p[j]) {
            seen[j] = true;
            cyc.back().push_back(j);
        }
    }
    return cyc;
}
// p applied k times (k up to 1e18): q[i] = p[p[...p[i]...]].
// Along its cycle, i moves k % len places. O(n)
vi applyK(const vi& p, ll k) {
    vi q(p.size());
    for (auto& c : cycles(p)) {
        int len = c.size();
        rep(j, 0, len) q[c[j]] = c[(j + k) % len];
    }
    return q;
}
