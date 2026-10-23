// union of intervals [s, e]: sorted, no overlaps. Intervals that
// touch ([1,3] and [3,5]) are merged too; < keeps them apart
vector<pair<ll, ll>> mergeIntervals(vector<pair<ll, ll>> iv) {
    sort(all(iv));
    vector<pair<ll, ll>> res;
    for (auto [s, e] : iv) {
        if (!res.empty() && s <= res.back().second)  // overlaps
            res.back().second = max(res.back().second, e);
        else res.push_back({s, e});
    }
    return res;
}
// length covered: add up e - s over the result
