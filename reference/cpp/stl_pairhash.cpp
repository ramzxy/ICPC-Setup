// unordered_map / unordered_set with pair keys need a hash
// (map<pii, int> works without one, in O(log n))
struct PairHash {
    size_t operator()(const pair<ll, ll>& p) const {
        unsigned long long x = p.first * 1000000007ULL + p.second;
        x ^= x >> 31;
        x *= 0x9e3779b97f4a7c15ULL;
        return x ^ (x >> 29);
    }
};
// unordered_map<pair<ll, ll>, int, PairHash> seen;
