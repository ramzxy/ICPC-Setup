// T = ll. Smallest SQUARED distance between two of the points
// (n >= 2). Sweep left to right; s = points with x > q.x - d,
// sorted by y. O(n log n)
ll closestPair(vector<P> p) {
    sort(all(p));
    set<pair<ll, ll>> s;   // {y, x}
    ll best = LLONG_MAX;
    int j = 0;
    for (P q : p) {
        ll d = sqrtl(best) + 1;   // farther points cannot win
        while (q.x - p[j].x >= d) s.erase({p[j].y, p[j].x}), j++;
        auto it = s.lower_bound({q.y - d, LLONG_MIN});
        for (; it != s.end() && it->first <= q.y + d; it++)
            best = min(best, norm(q - P{it->second, it->first}));
        s.insert({q.y, q.x});
    }
    return best;
}
