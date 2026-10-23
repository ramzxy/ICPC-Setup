// returns g = gcd(a, b) and sets x, y so that a * x + b * y = g
ll egcd(ll a, ll b, ll& x, ll& y) {
    if (b == 0) { x = 1, y = 0; return a; }
    ll g = egcd(b, a % b, y, x);
    y -= a / b * x;
    return g;
}
// a * x + b * y = c with a, b > 0. Returns false if no solution.
// Gives the smallest x >= 0, and its y. All solutions:
// x + k * (b / g), y - k * (a / g) for every integer k
bool solveLinear(ll a, ll b, ll c, ll& x, ll& y) {
    ll x0, y0, g = egcd(a, b, x0, y0);
    if (c % g != 0) return false;
    ll m = b / g;
    x = ((__int128)x0 * (c / g) % m + m) % m;
    y = (c - (__int128)a * x) / b;
    return true;
}
