// Checks the snippets of sections/math.tex against brute force.
#include "check.hpp"
#include "../code/cpp/math_int.cpp"
#include "../code/cpp/math_gcd.cpp"
#include "../code/cpp/math_frac.cpp"
#include "../code/cpp/math_egcd.cpp"
#include "../code/cpp/math_mod.cpp"
#include "../code/cpp/math_sieve.cpp"
#include "../code/cpp/math_factor.cpp"
#include "../code/cpp/math_divisors.cpp"
#include "../code/cpp/math_crt.cpp"
#include "../code/cpp/math_cycles.cpp"
#include "../code/cpp/math_matrix.cpp"
#include "../code/cpp/math_xor.cpp"

void testIntegers() {
    rep(iter, 0, 50000) {
        ll a = rnd(-1000, 1000), b = rnd(-30, 30);
        if (b == 0) continue;
        ll fl = -1001;  // brute floor: largest integer <= a / b
        while ((long double)(fl + 1) <= (long double)a / b) fl++;
        CHECK(floorDiv(a, b) == fl);
        ll ce = 1001;   // brute ceil: smallest integer >= a / b
        while ((long double)(ce - 1) >= (long double)a / b) ce--;
        CHECK(ceilDiv(a, b) == ce);
        ll m = rnd(1, 30), r = a;
        while (r < 0) r += m;
        while (r >= m) r -= m;
        CHECK(md(a, m) == r);
    }
    auto slowSqrt = [](ll n) {   // binary search, products in __int128
        ll lo = 0, hi = 2e9;
        while (lo < hi) {
            ll mid = (lo + hi + 1) / 2;
            if ((__int128)mid * mid <= n) lo = mid; else hi = mid - 1;
        }
        return lo;
    };
    rep(n, 0, 20000) CHECK(isqrt(n) == slowSqrt(n));
    rep(iter, 0, 100000) {
        ll n = rndll(0, (ll)1e18);
        CHECK(isqrt(n) == slowSqrt(n));
        ll k = rndll(0, (ll)1e9);   // near perfect squares
        for (ll d = -2; d <= 2; d++) {
            ll x = k * k + d;
            if (x >= 0 && x <= (ll)1e18) CHECK(isqrt(x) == slowSqrt(x));
        }
    }
    CHECK(isqrt((ll)1e18) == (ll)1e9);
    CHECK(isqrt((ll)1e18 - 1) == (ll)1e9 - 1);
    puts("  floorDiv, ceilDiv, md, isqrt OK");
}

void testGcdFrac() {
    rep(iter, 0, 20000) {
        int k = rnd(0, 5);
        vector<ll> a(k);
        for (ll& x : a) x = rnd(1, 20);
        ll want = 1;   // smallest positive multiple of all of them
        while (true) {
            bool ok = true;
            for (ll x : a) if (want % x != 0) ok = false;
            if (ok) break;
            want++;
        }
        CHECK(lcmAll(a) == want);
        ll LIM = rnd(1, 3000);
        CHECK(lcmAll(a, LIM) == (want <= LIM ? want : -1));
    }
    rep(iter, 0, 2000) {   // big values: compare with an exact __int128 lcm
        int k = rnd(1, 6);
        vector<ll> a(k);
        for (ll& x : a) x = rndll(1, (ll)1e12);
        __int128 l = 1;
        bool over = false;
        for (ll x : a) {
            l = l / std::gcd((ll)l, x) * x;
            if (l > (__int128)2e18) { over = true; break; }
        }
        CHECK(lcmAll(a) == (over ? -1 : (ll)l));
    }
    rep(iter, 0, 20000) {
        ll a = rnd(-30, 30), b = rnd(-30, 30), c = rnd(-30, 30), d = rnd(-30, 30);
        if (b == 0 || d == 0) continue;
        Frac x(a, b), y(c, d);
        for (Frac f : {x, y}) CHECK(f.q > 0 && std::gcd(f.p, f.q) == 1);
        CHECK(x.p * b == a * x.q);   // same value as a / b
        auto same = [](Frac f, ll p, ll q) { return f.p * q == p * f.q; };
        CHECK(same(x + y, a * d + c * b, b * d));
        CHECK(same(x - y, a * d - c * b, b * d));
        CHECK(same(x * y, a * c, b * d));
        if (c != 0) CHECK(same(x / y, a * d, b * c));
        CHECK((x < y) == ((long double)a / b < (long double)c / d));
        CHECK((x == y) == (a * d == c * b));
    }
    puts("  lcmAll, Frac OK");
}

void testEgcd() {
    rep(iter, 0, 100000) {
        ll a = rndll(0, (ll)1e12), b = rndll(0, (ll)1e12), x, y;
        if (iter % 2) a = rnd(0, 50), b = rnd(0, 50);
        ll g = egcd(a, b, x, y);
        CHECK(g == std::gcd(a, b));
        CHECK((__int128)a * x + (__int128)b * y == g);
    }
    rep(iter, 0, 100000) {
        ll a = rnd(1, 40), b = rnd(1, 40), c = rnd(-200, 200), x, y;
        ll want = -1;   // smallest x >= 0; checking x < b is enough
        rep(t, 0, b) if ((c - a * t) % b == 0) { want = t; break; }
        bool ok = solveLinear(a, b, c, x, y);
        CHECK(ok == (want != -1));
        if (ok) CHECK(x == want && a * x + b * y == c);
    }
    rep(iter, 0, 20000) {   // large values
        ll a = rndll(1, (ll)1e12), b = rndll(1, (ll)1e12), x, y;
        ll g = std::gcd(a, b), c = rndll(-(ll)1e17, (ll)1e17);
        if (iter % 2) c -= c % g;   // make it solvable
        bool ok = solveLinear(a, b, c, x, y);
        CHECK(ok == (c % g == 0));
        if (ok) CHECK(x >= 0 && x < b / g && (__int128)a * x + (__int128)b * y == c);
    }
    puts("  egcd, solveLinear OK");
}

void testMod() {
    rep(iter, 0, 20000) {
        ll m = iter % 3 ? rndll(2, (ll)3e9 - 1) : rnd(2, 50);
        ll b = rndll(-(ll)1e18, (ll)1e18);
        int e = rnd(0, 300);
        __int128 slow = 1;
        rep(i, 0, e) slow = slow * (((b % m) + m) % m) % m;
        CHECK(modpow(b, e, m) == (ll)slow);
        ll e1 = rndll(0, (ll)1e18), e2 = rndll(0, (ll)1e18);
        CHECK(modpow(b, e1 + e2, m) == (__int128)modpow(b, e1, m) * modpow(b, e2, m) % m);
    }
    rep(iter, 0, 20000) {
        ll a = rndll(1, MOD - 1);
        CHECK(a * inv(a) % MOD == 1);
        ll p = 998244353, x = rndll(1, p - 1);
        CHECK(x * inv(x, p) % p == 1);
    }
    for (ll p : {2, 3, 5, 7, 11, 13, 97}) rep(a, 1, p) CHECK(a * inv(a, p) % p == 1);
    CHECK(modpow(2, 10) == 1024 && modpow(5, 0) == 1);
    puts("  modpow, inv OK");
}

void testPrimes() {
    sieve();
    auto slowSpf = [](ll x) { for (ll d = 2; d * d <= x; d++) if (x % d == 0) return d; return x; };
    rep(x, 2, 100001) CHECK(spf[x] == slowSpf(x));
    rep(iter, 0, 20000) {
        int x = rnd(2, MAXV);
        CHECK(spf[x] == slowSpf(x));
    }
    CHECK(primes.size() == 664579);   // primes up to 1e7
    CHECK(upper_bound(all(primes), 1000000) - primes.begin() == 78498);
    CHECK(upper_bound(all(primes), 1000) - primes.begin() == 168);
    rep(i, 1, (int)primes.size()) CHECK(primes[i - 1] < primes[i]);
    for (int p : primes) CHECK(spf[p] == p);
    rep(iter, 0, 20000) {
        int x = iter < 100 ? iter + 1 : rnd(1, MAXV);
        vi f = factorSmall(x);
        ll prod = 1;
        rep(i, 0, (int)f.size()) {
            CHECK(slowSpf(f[i]) == f[i]);
            if (i) CHECK(f[i - 1] <= f[i]);
            prod *= f[i];
        }
        CHECK(prod == x);
        CHECK(isPrime(x) == (x >= 2 && spf[x] == x));
    }
    rep(x, -5, 200000) CHECK(isPrime(x) == (x >= 2 && spf[x] == x));
    CHECK(isPrime(999999999989LL) && !isPrime(999999999989LL * 3));
    CHECK(isPrime(1000000007) && !isPrime(1000003LL * 1000033LL));
    rep(iter, 0, 150) {
        ll n = iter < 50 ? iter + 1 : rndll(1, (ll)1e12);
        if (iter % 3 == 0 && iter >= 50) n = 1LL * primes[rnd(70000, 78000)] * primes[rnd(0, 78000)];
        auto f = factor(n);
        ll prod = 1;
        rep(i, 0, (int)f.size()) {
            auto [p, e] = f[i];
            CHECK(isPrime(p) && e >= 1);
            if (i) CHECK(f[i - 1].first < p);
            rep(j, 0, e) prod *= p;
        }
        CHECK(prod == n);
    }
    CHECK(factor(1).empty());
    CHECK((factor(360) == vector<pair<ll, int>>{{2, 3}, {3, 2}, {5, 1}}));
    puts("  sieve, factorSmall, isPrime, factor OK");
}

void testDivisors() {
    rep(n, 1, 3001) {
        vector<ll> want;
        rep(d, 1, n + 1) if (n % d == 0) want.push_back(d);
        CHECK(divisors(n) == want);
    }
    rep(iter, 0, 200) {
        ll n = rndll(1, (ll)1e12);
        auto d = divisors(n);
        ll cnt = 1;   // number of divisors = product of (e + 1)
        for (auto [p, e] : factor(n)) cnt *= e + 1;
        CHECK((ll)d.size() == cnt);
        rep(i, 0, (int)d.size()) {
            CHECK(n % d[i] == 0);
            if (i) CHECK(d[i - 1] < d[i]);
        }
    }
    vi cnt = divisorCounts(3000);
    rep(x, 1, 3001) CHECK(cnt[x] == (int)divisors(x).size());
    // the numbers in the formula table
    vi big = divisorCounts(1000000);
    CHECK(*max_element(big.begin() + 1, big.end()) == 240);
    CHECK(divisors(735134400).size() == 1344);
    CHECK(divisors(963761198400LL).size() == 6720);
    puts("  divisors, divisorCounts OK");
}

void testCrt() {
    rep(iter, 0, 100000) {
        ll m1 = rnd(1, 30), m2 = rnd(1, 30);
        ll a1 = rnd(0, m1 - 1), a2 = rnd(0, m2 - 1);
        ll want = -1;
        rep(x, 0, m1 * m2) if (x % m1 == a1 && x % m2 == a2) { want = x; break; }
        auto [x, l] = crt(a1, m1, a2, m2);
        CHECK(x == want);
        if (want != -1) CHECK(l == std::lcm(m1, m2));
    }
    rep(iter, 0, 20000) {   // big moduli with a common factor
        ll g = rnd(1, 1000);
        ll m1 = g * rnd(1, 1000000), m2 = g * rnd(1, 1000000);
        ll x0 = rndll(0, (ll)1e18);
        ll a1 = x0 % m1, a2 = x0 % m2;   // solvable by construction
        auto [x, l] = crt(a1, m1, a2, m2);
        CHECK(l == std::lcm(m1, m2) && x >= 0 && x < l);
        CHECK(x % m1 == a1 && x % m2 == a2 && x == x0 % l);
    }
    puts("  crt OK");
}

void testCycles() {
    rep(iter, 0, 20000) {
        int n = rnd(1, 8);
        vi p(n);
        iota(all(p), 0);
        shuffle(all(p), rng);
        auto cyc = cycles(p);
        vi seen(n, 0);
        for (auto& c : cyc) rep(j, 0, (int)c.size()) {
            seen[c[j]]++;
            CHECK(p[c[j]] == c[(j + 1) % c.size()]);
        }
        rep(i, 0, n) CHECK(seen[i] == 1);
        vi q(n);   // brute: apply p again and again
        iota(all(q), 0);
        rep(k, 0, 60) {
            CHECK(applyK(p, k) == q);
            rep(i, 0, n) q[i] = p[q[i]];
        }
        vector<ll> lens;   // order of p = lcm of cycle lengths
        for (auto& c : cyc) lens.push_back(c.size());
        ll order = 1;
        iota(all(q), 0);
        vi id = q;
        while (true) {
            rep(i, 0, n) q[i] = p[q[i]];
            if (q == id) break;
            order++;
        }
        CHECK(lcmAll(lens) == order);
        ll k = rndll(0, (ll)1e18);
        CHECK(applyK(p, k) == applyK(p, k % order));
    }
    puts("  cycles, applyK OK");
}

void testMatrix() {
    vector<ll> fib = {0, 1};
    rep(i, 2, 2000) fib.push_back((fib[i - 1] + fib[i - 2]) % MOD);
    rep(n, 0, 2000) CHECK(matpow({{1, 1}, {1, 0}}, n)[0][1] == fib[n]);
    rep(iter, 0, 300) {
        int k = rnd(1, 4);
        Mat A(k, vector<ll>(k));
        for (auto& row : A) for (ll& x : row) x = rndll(0, MOD - 1);
        Mat R(k, vector<ll>(k, 0));
        rep(i, 0, k) R[i][i] = 1;
        rep(e, 0, 40) {
            CHECK(matpow(A, e) == R);
            Mat S(k, vector<ll>(k, 0));   // R = R * A by hand
            rep(i, 0, k) rep(j, 0, k) rep(l, 0, k) S[i][j] = (S[i][j] + (__int128)R[i][l] * A[l][j]) % MOD;
            R = S;
        }
        ll e1 = rndll(0, (ll)1e18), e2 = rndll(0, (ll)1e18);
        CHECK(matpow(A, e1 + e2) == matmul(matpow(A, e1), matpow(A, e2)));
    }
    // the recipe in the text: f(n) = 2 f(n-1) + 3 f(n-2) (+ 5), from f(0), f(1)
    rep(iter, 0, 200) {
        ll f0 = rnd(0, 100), f1 = rnd(0, 100);
        vector<ll> f = {f0, f1}, g = {f0, f1};
        rep(i, 2, 300) {
            f.push_back((2 * f[i - 1] + 3 * f[i - 2]) % MOD);
            g.push_back((2 * g[i - 1] + 3 * g[i - 2] + 5) % MOD);
        }
        rep(n, 1, 300) {
            Mat R = matpow({{2, 3}, {1, 0}}, n - 1);
            CHECK((R[0][0] * f1 + R[0][1] * f0) % MOD == f[n]);
            Mat T = matpow({{2, 3, 5}, {1, 0, 0}, {0, 0, 1}}, n - 1);
            CHECK((T[0][0] * f1 + T[0][1] * f0 + T[0][2]) % MOD == g[n]);
        }
    }
    puts("  matpow OK");
}

void testXor() {
    rep(iter, 0, 5000) {
        int k = rnd(0, 8), bits = rnd(1, 7);
        XorBasis xb;
        set<ll> span = {0};   // brute: every subset xor so far
        rep(i, 0, k) {
            ll x = rnd(0, (1 << bits) - 1);
            CHECK(xb.insert(x) == !span.count(x));
            set<ll> next = span;
            for (ll s : span) next.insert(s ^ x);
            span = next;
            rep(t, 0, 1 << bits) {
                ll mn = LLONG_MAX, mx = -1;
                for (ll s : span) mn = min(mn, t ^ s), mx = max(mx, t ^ s);
                CHECK(xb.minXor(t) == mn && xb.maxXor(t) == mx);
            }
        }
    }
    XorBasis xb;   // the top bits work too
    xb.insert((1LL << 60) | 5);
    xb.insert(3);
    CHECK(xb.maxXor(0) == ((1LL << 60) | 6) && xb.minXor((1LL << 60) | 4) == 1);
    puts("  XorBasis OK");
}

int main() {
    testIntegers();
    testGcdFrac();
    testEgcd();
    testMod();
    testPrimes();
    testDivisors();
    testCrt();
    testCycles();
    testMatrix();
    testXor();
}
