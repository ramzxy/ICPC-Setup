// sieve: spf[x] = smallest prime factor of x (2 <= x <= MAXV),
// primes = all primes <= MAXV. x is prime iff spf[x] == x
const int MAXV = 1e7;
vi spf(MAXV + 1, 0), primes;
void sieve() {
    for (int i = 2; i <= MAXV; i++) if (spf[i] == 0) {
        primes.push_back(i);
        for (int j = i; j <= MAXV; j += i)
            if (spf[j] == 0) spf[j] = i;
    }
}
// prime factors of x (1 <= x <= MAXV) after sieve(), small first,
// with repeats: 12 gives {2, 2, 3}. O(log x)
vi factorSmall(int x) {
    vi f;
    while (x > 1) { f.push_back(spf[x]); x /= spf[x]; }
    return f;
}
