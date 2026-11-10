// xor basis: which values are the xor of some subset of the
// numbers inserted so far (numbers < 2^61). O(61) per operation
struct XorBasis {
    ll b[61] = {};  // b[i]: a number with highest bit i, or 0
    bool insert(ll x) {  // false: x was already a subset xor
        for (int i = 60; i >= 0; i--) if (x >> i & 1) {
            if (b[i] == 0) { b[i] = x; return true; }
            x ^= b[i];
        }
        return false;
    }
    // smallest / largest value of x ^ (xor of some subset)
    ll minXor(ll x) {
        for (int i = 60; i >= 0; i--) x = min(x, x ^ b[i]);
        return x;
    }
    ll maxXor(ll x) {
        for (int i = 60; i >= 0; i--) x = max(x, x ^ b[i]);
        return x;
    }
};
