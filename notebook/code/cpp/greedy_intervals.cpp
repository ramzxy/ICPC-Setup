// most intervals [s, e) with no two overlapping (touching is ok).
// Take the one that ends first, skip all that start before its end
int maxDisjoint(vector<pair<ll, ll>> iv) {  // iv[i] = {s, e}
    sort(all(iv), [](auto& a, auto& b) {
        return a.second < b.second;
    });
    int cnt = 0;
    ll last = -INF;  // end of the last interval we took
    for (auto [s, e] : iv)
        if (s >= last) { cnt++; last = e; }
    return cnt;
}
