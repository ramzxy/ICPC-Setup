// nCr(n, k) mod MOD for 0 <= n < MAXN, O(1) after initFact().
// Needs MOD (a prime >= MAXN) and modpow from the Math chapter
const int MAXN = 1e6 + 1;
ll fact[MAXN], ifact[MAXN];  // n! and 1 / n!
void initFact() {
    fact[0] = 1;
    rep(i, 1, MAXN) fact[i] = fact[i - 1] * i % MOD;
    ifact[MAXN - 1] = modpow(fact[MAXN - 1], MOD - 2);
    for (int i = MAXN - 1; i > 0; i--)
        ifact[i - 1] = ifact[i] * i % MOD;  // 1/(i-1)! = i * 1/i!
}
ll nCr(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[k] % MOD * ifact[n - k] % MOD;
}
