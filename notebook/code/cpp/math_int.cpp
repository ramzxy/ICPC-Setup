// floor and ceil of a / b for all signs (b != 0). Plain a / b
// rounds towards 0: -7 / 2 == -3, and -7 % 3 == -1
ll floorDiv(ll a, ll b) {
    ll q = a / b;
    if (a % b != 0 && (a < 0) != (b < 0)) q--;
    return q;
}
ll ceilDiv(ll a, ll b) { return -floorDiv(-a, b); }
// a mod m in [0, m), also when a < 0: md(-7, 3) == 2
ll md(ll a, ll m) { return (a % m + m) % m; }
// exact floor(sqrt(n)) for 0 <= n <= 1e18 (sqrt alone can be 1 off)
ll isqrt(ll n) {
    ll r = sqrtl(n);
    while (r * r > n) r--;
    while ((r + 1) * (r + 1) <= n) r++;
    return r;
}
