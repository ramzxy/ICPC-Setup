// all divisors of n >= 1, sorted. O(sqrt(n)): they come in pairs
// d * (n / d) = n with d <= sqrt(n)
vector<ll> divisors(ll n) {
    vector<ll> small, big;
    for (ll d = 1; d * d <= n; d++) if (n % d == 0) {
        small.push_back(d);
        if (d != n / d) big.push_back(n / d);
    }
    small.insert(small.end(), big.rbegin(), big.rend());
    return small;
}
// cnt[x] = number of divisors of x, for every x <= N. Visits all
// multiples of every d: N/1 + N/2 + ... + N/N = O(N log N) steps
vi divisorCounts(int N) {
    vi cnt(N + 1, 0);
    for (int d = 1; d <= N; d++)
        for (int x = d; x <= N; x += d) cnt[x]++;
    return cnt;
}
