// try every order of n items (n <= 10: 10! = 3.6e6 orders).
// Example: shortest route through all n cities, d = distances.
vi p(n);
iota(all(p), 0);   // 0, 1, ..., n-1: must start sorted!
ll best = INF;
do {
    ll len = 0;
    rep(i, 0, n - 1) len += d[p[i]][p[i + 1]];
    best = min(best, len);
} while (next_permutation(all(p)));
