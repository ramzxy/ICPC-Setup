// most intervals [s, e) open at the same moment = rooms needed.
// Events: +1 at s, -1 at e. At equal times {t, -1} sorts before
// {t, +1}, so [1,3) and [3,5) can share a room
int maxOverlap(const vector<pair<ll, ll>>& iv) {
    vector<pair<ll, int>> ev;
    for (auto [s, e] : iv) {
        ev.push_back({s, +1});
        ev.push_back({e, -1});
    }
    sort(all(ev));
    int cur = 0, best = 0;
    for (auto [t, d] : ev) {
        cur += d;
        best = max(best, cur);
    }
    return best;
}
