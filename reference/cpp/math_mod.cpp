const ll MOD = 1e9 + 7;  // a prime; 998244353 is another one
// b^e mod m in O(log e). Needs m < 3e9, so that m * m fits in ll
ll modpow(ll b, ll e, ll m = MOD) {
    ll r = 1;
    b = (b % m + m) % m;
    while (e > 0) {
        if (e & 1) r = r * b % m;
        b = b * b % m;
        e >>= 1;
    }
    return r;
}
// 1 / a mod p, for p prime and a % p != 0 (Fermat: a^(p-2))
ll inv(ll a, ll p = MOD) { return modpow(a, p - 2, p); }
