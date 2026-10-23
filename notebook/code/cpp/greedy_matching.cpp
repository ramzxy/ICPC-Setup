// person i accepts an item of size lo..hi; each item goes to at
// most one person. Most people served: smallest hi first, gets
// the smallest free item >= lo. people[i] = {hi, lo}
int maxMatch(vector<pair<ll, ll>> people, vector<ll> items) {
    sort(all(people));  // smallest hi first
    multiset<ll> avail(all(items));
    int cnt = 0;
    for (auto [hi, lo] : people) {
        auto it = avail.lower_bound(lo);
        if (it != avail.end() && *it <= hi) {
            avail.erase(it);  // erase(it), not erase(value)!
            cnt++;
        }
    }
    return cnt;
}
