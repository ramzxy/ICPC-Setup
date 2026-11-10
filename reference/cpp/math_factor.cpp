// trial division up to sqrt(n): 1e6 steps for n = 1e12
bool isPrime(ll n) {
    if (n < 2) return false;
    for (ll d = 2; d * d <= n; d++) if (n % d == 0) return false;
    return true;
}
// prime factorisation as {prime, exponent}, primes increasing:
// 360 = 2^3 * 3^2 * 5 gives {{2, 3}, {3, 2}, {5, 1}}. n = 1: {}
vector<pair<ll, int>> factor(ll n) {
    vector<pair<ll, int>> f;
    for (ll p = 2; p * p <= n; p++) if (n % p == 0) {
        int e = 0;
        while (n % p == 0) n /= p, e++;
        f.push_back({p, e});
    }
    if (n > 1) f.push_back({n, 1});  // the rest is one big prime
    return f;
}
