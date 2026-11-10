// polynomial hash mod 2^61 - 1. After O(n) setup, get(l, r) is the
// hash of s[l..r-1] in O(1). Equal strings give equal hashes;
// different ones almost never do (chance about n / 2^61)
using ull = unsigned long long;
const ull HMOD = (1ULL << 61) - 1, HB = 1e9 + 7;  // HB: big, < HMOD
ull hmul(ull a, ull b) { return (__uint128_t)a * b % HMOD; }
struct Hash {
    vector<ull> h, pw;   // h[i] = hash of s[0..i-1], pw[i] = HB^i
    Hash(const string& s) : h(s.size() + 1), pw(s.size() + 1) {
        pw[0] = 1;
        rep(i, 0, (int)s.size()) {
            h[i + 1] = (hmul(h[i], HB) + s[i]) % HMOD;
            pw[i + 1] = hmul(pw[i], HB);
        }
    }
    ull get(int l, int r) {   // hash of s[l..r-1]
        return (h[r] + HMOD - hmul(h[l], pw[r - l])) % HMOD;
    }
};
// Hash hs(s); if (hs.get(i, i + L) == hs.get(j, j + L)) same text
// two strings: one Hash for each, then compare get() values
