// Checks the snippets of sections/searching.tex (and the input idioms of
// sections/contest.tex) against brute force.
#include "check.hpp"
#include "../code/cpp/binary_search.cpp"
#include "../code/cpp/ternary.cpp"
#include "../code/cpp/mono_stack.cpp"
#include "../code/cpp/histogram.cpp"
#include "../code/cpp/sliding_min.cpp"

pair<vi, vector<ll>> compressOf(const vector<ll>& a) {
    int n = a.size();
#include "../code/cpp/compress.cpp"
    return {id, vals};
}

template <class F> ll peakOf(ll lo, ll hi, F f) {
#include "../code/cpp/peak_int.cpp"
    return peak;
}
template <class F> double realSearch(F ok) {
#include "../code/cpp/binary_search_real.cpp"
    return hi;
}
vector<ll> prefixOf(const vector<ll>& a) {
    int n = a.size();
#include "../code/cpp/prefix.cpp"
    return pre;
}
void diffApply(vector<ll>& a, const vector<tuple<int, int, ll>>& updates) {
    int n = a.size();
#include "../code/cpp/diff_array.cpp"
}
int longestWindow(const vector<ll>& a, ll K) {
    int n = a.size();
#include "../code/cpp/two_pointers.cpp"
    return best;
}
void readingIdioms() {   // only compiled, never run
#include "../code/cpp/io.cpp"
}

void testSearching() {
    rep(iter, 0, 2000) {
        ll lo = rnd(-50, 50), hi = lo + rnd(0, 60);
        ll t = rndll(lo, hi + 1);   // hi + 1 means "never true"
        CHECK(firstTrue(lo, hi, [&](ll x) { return x >= t; }) == t);
        // "largest x with ok(x)" recipe, ok(x) = x < t
        CHECK(firstTrue(lo, hi, [&](ll x) { return !(x < t); }) - 1 == t - 1);
    }
    ll big = 1234567890123456789LL;
    CHECK(firstTrue(0, (ll)2e18, [&](ll x) { return x >= big; }) == big);
    rep(iter, 0, 2000) {   // strictly up, then strictly down
        int n = rnd(1, 30), p = rnd(0, n - 1);
        vector<ll> v(n);
        v[p] = 1000;
        for (int i = p - 1; i >= 0; i--) v[i] = v[i + 1] - rnd(1, 5);
        for (int i = p + 1; i < n; i++) v[i] = v[i - 1] - rnd(1, 5);
        CHECK(peakOf(0, n - 1, [&](ll x) { return v[x]; }) == p);
    }
    rep(iter, 0, 200) {
        double a = rnd(-10000, 10000) / 100.0;
        double x = ternaryMax(-100, 100, [&](double y) { return 5 - (y - a) * (y - a); });
        CHECK(fabs(x - a) < 1e-6);
        double c = rnd(0, 1000000) / 7.0;
        double r = realSearch([&](double y) { return y * y >= c; });
        CHECK(fabs(r - sqrt(c)) < 1e-6);
    }
    puts("  binary search, peak search, ternary search OK");
}

void testArrays() {
    rep(iter, 0, 2000) {
        int n = rnd(0, 12);
        vector<ll> a(n);
        for (ll& x : a) x = rnd(0, 9);
        auto pre = prefixOf(a);
        rep(l, 0, n) rep(r, l, n) {
            ll s = 0;
            rep(i, l, r + 1) s += a[i];
            CHECK(pre[r + 1] - pre[l] == s);
        }
        ll K = rnd(0, 30);
        int best = 0;
        rep(l, 0, n) {
            ll s = 0;
            rep(r, l, n) { s += a[r]; if (s <= K) best = max(best, r - l + 1); }
        }
        CHECK(longestWindow(a, K) == best);
        if (n > 0) {
            vector<tuple<int, int, ll>> ups;
            vector<ll> want = a;
            rep(i, 0, rnd(0, 6)) {
                int l = rnd(0, n - 1), r = rnd(l, n - 1);
                ll v = rnd(-5, 5);
                ups.push_back({l, r, v});
                rep(j, l, r + 1) want[j] += v;
            }
            vector<ll> got = a;
            diffApply(got, ups);
            CHECK(got == want);
        }
    }
    rep(iter, 0, 300) {
        int R = rnd(1, 6), C = rnd(1, 6);
        vector<vector<ll>> g(R, vector<ll>(C));
        for (auto& row : g) for (ll& x : row) x = rnd(-9, 9);
#include "../code/cpp/prefix2d.cpp"
        rep(r1, 0, R) rep(r2, r1, R) rep(c1, 0, C) rep(c2, c1, C) {
            ll s = 0;
            rep(i, r1, r2 + 1) rep(j, c1, c2 + 1) s += g[i][j];
            CHECK(rect(r1, c1, r2, c2) == s);
        }
    }
    puts("  prefix sums (1D, 2D), difference array, two pointers OK");
}

void testStacks() {
    rep(iter, 0, 3000) {
        int n = rnd(0, 12);
        vector<ll> a(n);
        for (ll& x : a) x = rnd(0, 6);   // small values: many ties
        auto [id, vals] = compressOf(a);
        rep(i, 0, n) {
            CHECK(vals[id[i]] == a[i]);
            rep(j, 0, n) CHECK((a[i] < a[j]) == (id[i] < id[j]));
        }
        rep(i, 0, (int)vals.size() - 1) CHECK(vals[i] < vals[i + 1]);
        vi nxt = nextGreater(a);
        rep(i, 0, n) {
            int want = n;
            for (int j = n - 1; j > i; j--) if (a[j] > a[i]) want = j;
            CHECK(nxt[i] == want);
        }
        ll best = 0;   // every range [l, r], height = its minimum
        rep(l, 0, n) {
            ll lo = LLONG_MAX;
            rep(r, l, n) { lo = min(lo, a[r]); best = max(best, lo * (r - l + 1)); }
        }
        CHECK(largestRect(a) == best);
        if (n > 0) {
            int k = rnd(1, n);
            auto mn = slidingMin(a, k);
            CHECK((int)mn.size() == n - k + 1);
            rep(j, 0, n - k + 1) CHECK(mn[j] == *min_element(a.begin() + j, a.begin() + j + k));
        }
    }
    puts("  coordinate compression, monotonic stack, histogram, sliding min OK");
}

int main() {
    testSearching();
    testArrays();
    testStacks();
}
