// Checks the snippets of sections/probability.tex against brute force.
#include "check.hpp"
#include "../code/cpp/math_mod.cpp"   // MOD, modpow, inv
#include "../code/cpp/prob_binom.cpp"
#include "../code/cpp/prob_incl_excl.cpp"
#include "../code/cpp/prob_modfrac.cpp"

vector<vector<ll>> pascal() {
#include "../code/cpp/prob_pascal.cpp"
    return C;
}
vector<double> diceDP(int n, int k) {
#include "../code/cpp/prob_expect_dp.cpp"
    return E;
}
vector<double> successDP(int n, double p) {
#include "../code/cpp/prob_dp.cpp"
    return dp;
}

void testBinomials() {
    initFact();
    int L = 1500;   // Pascal's triangle mod MOD, built independently
    vector<vector<ll>> P(L, vector<ll>(L, 0));
    rep(n, 0, L) {
        P[n][0] = 1;
        rep(k, 1, n + 1) P[n][k] = (P[n - 1][k - 1] + P[n - 1][k]) % MOD;
    }
    rep(n, 0, L) rep(k, -2, n + 3) CHECK(nCr(n, k) == (k >= 0 && k <= n ? P[n][k] : 0));
    rep(iter, 0, 100000) {   // large n: Pascal's rule must still hold
        int n = rnd(1, MAXN - 1), k = rnd(0, n);
        CHECK(nCr(n, k) == (nCr(n - 1, k - 1) + nCr(n - 1, k)) % MOD);
    }
    CHECK(nCr(MAXN - 1, 0) == 1 && nCr(MAXN - 1, MAXN - 1) == 1);
    CHECK(nCr(1000000, 1) == 1000000);
    auto C = pascal();   // exact values: multiply and divide step by step
    rep(n, 0, 67) rep(k, 0, n + 1) {
        __int128 c = 1;
        rep(i, 1, k + 1) c = c * (n - k + i) / i;
        CHECK(C[n][k] == (ll)c);
    }
    puts("  nCr mod p, Pascal OK");
}

void testInclExcl() {
    rep(iter, 0, 3000) {
        ll n = rnd(0, 2000);
        int k = rnd(0, 6);
        vector<ll> a(k);
        for (ll& x : a) x = rnd(1, 40);
        ll want = 0;
        rep(x, 1, n + 1) {
            bool any = false;
            for (ll y : a) if (x % y == 0) any = true;
            want += any;
        }
        CHECK(countDivisible(n, a) == want);
    }
    rep(iter, 0, 200) {   // large values, k = 20 (lcm overflow guard)
        ll n = rnd(1, 1000000000);
        vector<ll> a(20);
        for (ll& x : a) x = rnd(1, 1000000000);
        a[0] = rnd(1, 3);
        ll s = countDivisible(n, a);
        CHECK(s >= n / a[0] && s <= n);
    }
    CHECK(countDivisible(30, {2, 3, 5}) == 22);
    puts("  inclusion-exclusion OK");
}

void testExpectation() {
    rep(n, 0, 15) rep(k, 1, 7) {
        // brute: value iteration E <- 1 + sum over throws, until it stops changing
        vector<double> e(n + 1, 0);
        rep(it, 0, 3000) {
            vector<double> ne(n + 1, 0);
            rep(i, 0, n) {
                ne[i] = 1;
                rep(r, 1, k + 1) ne[i] += (i + r <= n ? e[i + r] : e[i]) / k;
            }
            e = ne;
        }
        auto E = diceDP(n, k);
        rep(i, 0, n + 1) CHECK(fabs(E[i] - e[i]) < 1e-9 * max(1.0, e[i]));
    }
    CHECK(fabs(diceDP(1, 6)[0] - 6) < 1e-9);   // need exactly a 1: 1/p = 6 throws
    rep(iter, 0, 300) {
        int n = rnd(0, 12);
        double p = rnd(0, 1000) / 1000.0;
        auto dp = successDP(n, p);
        vector<double> want(n + 1, 0);   // brute: every outcome of the n tries
        rep(mask, 0, 1 << n) {
            int s = __builtin_popcount(mask);
            want[s] += pow(p, s) * pow(1 - p, n - s);
        }
        CHECK((int)dp.size() == n + 1);
        double total = 0;
        rep(j, 0, n + 1) {
            CHECK(fabs(dp[j] - want[j]) < 1e-12);
            total += dp[j];
        }
        CHECK(fabs(total - 1) < 1e-9);
    }
    puts("  expected-value DP, probability DP OK");
}

void testFrac() {
    CHECK(frac(1, 2) == (MOD + 1) / 2 && frac(3, 1) == 3 && frac(0, 5) == 0);
    rep(iter, 0, 100000) {
        ll a = rnd(-1000, 1000), b = rnd(1, 1000), c = rnd(-1000, 1000), d = rnd(1, 1000);
        ll x = frac(a, b), y = frac(c, d);
        CHECK(x >= 0 && x < MOD);
        CHECK(x * b % MOD == (a % MOD + MOD) % MOD);
        CHECK((x + y) % MOD == frac(a * d + c * b, b * d));
        CHECK(x * y % MOD == frac(a * c, b * d));
        CHECK(frac(a * 7, b * 7) == x);
    }
    puts("  fractions mod p OK");
}

void testFormulas() {   // the formulas printed in the text, against brute force
    rep(k, 1, 5) rep(s, 0, 13) rep(u, 0, 6) {
        int all = 0, bounded = 0, positive = 0;   // count x_1 + ... + x_k = s by brute force
        vi x(k, 0);
        while (true) {
            int sum = 0, mx = 0, mn = 99;
            for (int v : x) sum += v, mx = max(mx, v), mn = min(mn, v);
            if (sum == s) all++, bounded += mx <= u, positive += mn >= 1;
            int i = 0;   // next x in 0..s per position
            while (i < k && x[i] == s) x[i++] = 0;
            if (i == k) break;
            x[i]++;
        }
        CHECK(all == nCr(s + k - 1, k - 1));
        CHECK(positive == (s >= 1 ? nCr(s - 1, k - 1) : 0));
        ll f = 0;   // stars and bars with x_i <= u, by inclusion-exclusion
        rep(j, 0, k + 1) f += (j % 2 ? -1 : 1) * nCr(k, j) * nCr(s - j * (u + 1) + k - 1, k - 1);
        CHECK(bounded == f);
    }
    vector<ll> D = {1, 0}, want = {1, 0, 1, 2, 9, 44, 265};
    rep(n, 2, 9) D.push_back((n - 1) * (D[n - 1] + D[n - 2]));
    rep(n, 0, 9) {
        vi p(n);
        iota(all(p), 0);
        ll cnt = 0;
        do {
            bool fixed = false;
            rep(i, 0, n) if (p[i] == i) fixed = true;
            cnt += !fixed;
        } while (next_permutation(all(p)));
        CHECK(D[n] == cnt);
        if (n < 7) CHECK(D[n] == want[n]);
    }
    vector<ll> cat = {1, 1, 2, 5, 14, 42, 132, 429, 1430};
    rep(n, 0, 9) {
        CHECK(nCr(2 * n, n) * inv(n + 1) % MOD == cat[n]);
        ll balanced = 0;   // bracket strings with n pairs
        rep(mask, 0, 1 << (2 * n)) {
            int depth = 0;
            bool ok = true;
            rep(i, 0, 2 * n) {
                depth += (mask >> i & 1) ? 1 : -1;
                if (depth < 0) ok = false;
            }
            balanced += ok && depth == 0;
        }
        CHECK(balanced == cat[n]);
        if (n > 0) {
            ll conv = 0;
            rep(i, 0, n) conv += cat[i] * cat[n - 1 - i];
            CHECK(conv == cat[n]);
        }
    }
    puts("  counting formulas in the text OK");
}

int main() {
    testBinomials();
    testInclExcl();
    testExpectation();
    testFrac();
    testFormulas();
}
