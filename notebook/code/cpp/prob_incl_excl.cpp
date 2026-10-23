// how many x in 1..n are divisible by at least one of a[0..k-1]?
// Inclusion-exclusion over all subsets S of a (k <= 20): add
// n / lcm(S) if |S| is odd, subtract it if even. n, a[i] <= 1e9
ll countDivisible(ll n, const vector<ll>& a) {
    int k = a.size();
    ll total = 0;
    rep(mask, 1, 1 << k) {
        ll l = 1;
        rep(i, 0, k) if (mask >> i & 1) {
            l = l / gcd(l, a[i]) * a[i];
            if (l > n) break;  // n / l is 0 from here on
        }
        if (__builtin_popcount(mask) % 2 == 1) total += n / l;
        else total -= n / l;
    }
    return total;
}
