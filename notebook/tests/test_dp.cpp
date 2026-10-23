// Checks the snippets of sections/dp.tex against brute force.
#include "check.hpp"
#include "../code/cpp/dp_knapsack.cpp"
#include "../code/cpp/dp_lis.cpp"
#include "../code/cpp/dp_lcs.cpp"
#include "../code/cpp/dp_edit.cpp"
#include "../code/cpp/dp_wordbreak.cpp"
#include "../code/cpp/dp_weighted.cpp"
#include "../code/cpp/dp_interval.cpp"
#include "../code/cpp/dp_bitmask.cpp"
#include "../code/cpp/dp_assign.cpp"
#include "../code/cpp/dp_split.cpp"
namespace frog {   // snippets with globals get their own namespace
#include "../code/cpp/dp_memo.cpp"
}
namespace digits {
#include "../code/cpp/dp_digits.cpp"
}
namespace nondecr {   // the "upper_bound here" comment of dp_lis
#define lower_bound upper_bound
#include "../code/cpp/dp_lis.cpp"
#undef lower_bound
}

const ll M = 1e9 + 7;

ll kadane(const vector<ll>& a) {
#include "../code/cpp/dp_kadane.cpp"
    return best;
}
vector<bool> subsetSums(const vi& w) {
#include "../code/cpp/dp_subset_sum.cpp"
    vector<bool> res(100001);
    rep(s, 0, 100001) res[s] = can[s];
    return res;
}
struct Coins { vector<ll> fewest, ways, order; };
Coins coinChange(const vi& coins, int S) {
#include "../code/cpp/dp_coins.cpp"
    return {fewest, ways, order};
}
ll gridPaths(const vector<string>& g) {
    int R = g.size(), C = g[0].size();
#include "../code/cpp/dp_grid.cpp"
    return ways[R - 1][C - 1];
}
vector<ll> stairs(int n, int k) {
#include "../code/cpp/dp_prefix.cpp"
    return dp;
}

void testMemoKadane() {
    rep(iter, 0, 2000) {
        int n = rnd(1, 10), k = rnd(1, 4);
        vector<ll> h(n);
        for (ll& x : h) x = rnd(0, 20);
        ll best = INF;   // every set of stones used, as a bitmask
        rep(mask, 0, 1 << n) {
            if (!(mask & 1) || !(mask >> (n - 1) & 1)) continue;
            ll cost = 0;
            int last = 0;
            bool ok = true;
            rep(i, 1, n) if (mask >> i & 1) {
                if (i - last > k) ok = false;
                cost += abs(h[i] - h[last]);
                last = i;
            }
            if (ok) best = min(best, cost);
        }
        frog::n = n, frog::k = k, frog::h = h;
        frog::memo.assign(n, -1);
        CHECK(frog::solve(0) == best);
        vector<ll> a(n);
        for (ll& x : a) x = rnd(-10, 10);
        ll bs = -INF;
        rep(l, 0, n) {
            ll s = 0;
            rep(r, l, n) s += a[r], bs = max(bs, s);
        }
        CHECK(kadane(a) == bs);
    }
    puts("  memoized recursion, best subarray OK");
}

void testKnapsack() {
    rep(iter, 0, 2000) {
        int n = rnd(0, 10), W = rnd(0, 30);
        vector<pair<int, ll>> items(n);
        for (auto& [w, v] : items) w = rnd(0, 12), v = rnd(0, 20);
        ll best = 0;
        vi ws;
        for (auto [w, v] : items) ws.push_back(w);
        set<int> sums;
        rep(mask, 0, 1 << n) {
            ll tw = 0, tv = 0;
            rep(i, 0, n) if (mask >> i & 1) tw += items[i].first, tv += items[i].second;
            if (tw <= W) best = max(best, tv);
            sums.insert(tw);
        }
        CHECK(knapsack(items, W) == best);
        auto can = subsetSums(ws);
        rep(s, 0, 200) CHECK(can[s] == (sums.count(s) > 0));
    }
    puts("  0/1 knapsack, subset sum OK");
}

void countOrdered(const vi& coins, int left, ll& cnt) {   // every sequence of coins
    if (left == 0) { cnt++; return; }
    for (int c : coins) if (c <= left) countOrdered(coins, left - c, cnt);
}
void testCoins() {
    rep(iter, 0, 1000) {
        int n = rnd(1, 4), S = rnd(0, 14);
        set<int> cs;
        while ((int)cs.size() < n) cs.insert(rnd(1, 8));
        vi coins(all(cs));
        shuffle(all(coins), rng);
        Coins got = coinChange(coins, S);
        rep(s, 0, S + 1) {
            // try every count of every coin type
            ll fewest = INF, ways = 0;
            vi cnt(n, 0);
            function<void(int, int)> go = [&](int i, int left) {
                if (i == n) {
                    if (left == 0) {
                        ways++;
                        fewest = min(fewest, (ll)accumulate(all(cnt), 0));
                    }
                    return;
                }
                for (cnt[i] = 0; cnt[i] * coins[i] <= left; cnt[i]++)
                    go(i + 1, left - cnt[i] * coins[i]);
                cnt[i] = 0;
            };
            go(0, s);
            ll ord = 0;
            countOrdered(coins, s, ord);
            CHECK(got.fewest[s] == fewest);
            CHECK(got.ways[s] == ways % M);
            CHECK(got.order[s] == ord % M);
        }
    }
    // mod really taken: ways to pay 1000 with 1 and 2 in any order = Fibonacci(1001)
    Coins big = coinChange({1, 2}, 1000);
    ll f0 = 1, f1 = 1;   // order[0], order[1]
    rep(i, 2, 1001) { ll f2 = (f0 + f1) % M; f0 = f1; f1 = f2; }
    CHECK(big.order[1000] == f1 && big.ways[1000] == 501 && big.fewest[1000] == 500);
    // ways with coins 1..30 for 2000 is far above the mod. p[i][s] = ways using coins 1..i
    vi c30(30);
    iota(all(c30), 1);
    vector<vector<ll>> p(31, vector<ll>(2001, 0));
    p[0][0] = 1;
    rep(i, 1, 31) rep(s, 0, 2001) p[i][s] = (p[i - 1][s] + (s >= i ? p[i][s - i] : 0)) % M;
    CHECK(coinChange(c30, 2000).ways[2000] == p[30][2000]);
    puts("  coin change (fewest, ways, ordered ways) OK");
}

bool isIncreasing(const vector<ll>& a, const vi& idx, bool strict) {
    rep(i, 1, (int)idx.size()) {
        if (idx[i - 1] >= idx[i]) return false;
        if (strict ? a[idx[i - 1]] >= a[idx[i]] : a[idx[i - 1]] > a[idx[i]]) return false;
    }
    return true;
}
void testLis() {
    rep(iter, 0, 3000) {
        int n = rnd(0, 12);
        vector<ll> a(n);
        for (ll& x : a) x = rnd(0, iter % 2 ? 5 : 30);   // many repeats half the time
        int bestS = 0, bestN = 0;
        rep(mask, 0, 1 << n) {
            vi idx;
            rep(i, 0, n) if (mask >> i & 1) idx.push_back(i);
            int sz = idx.size();
            if (isIncreasing(a, idx, true)) bestS = max(bestS, sz);
            if (isIncreasing(a, idx, false)) bestN = max(bestN, sz);
        }
        vi s = lis(a), ns = nondecr::lis(a);
        CHECK((int)s.size() == bestS && isIncreasing(a, s, true));
        CHECK((int)ns.size() == bestN && isIncreasing(a, ns, false));
    }
    puts("  LIS (strict, non-decreasing) OK");
}

string randStr(int maxLen, char maxCh) {
    string s(rnd(0, maxLen), 'a');
    for (char& c : s) c = 'a' + rnd(0, maxCh - 'a');
    return s;
}
bool isSubseq(const string& t, const string& s) {   // t inside s?
    size_t j = 0;
    for (char c : s) if (j < t.size() && t[j] == c) j++;
    return j == t.size();
}
int bfsEdit(const string& a, const string& b) {   // shortest path over all strings
    size_t maxLen = max(a.size(), b.size()) + 1;
    map<string, int> dist{{a, 0}};
    queue<string> q;
    q.push(a);
    while (!q.empty()) {
        string u = q.front(); q.pop();
        if (u == b) return dist[u];
        vector<string> next;
        rep(i, 0, (int)u.size() + 1) for (char c = 'a'; c <= 'c'; c++) {
            if (u.size() < maxLen) next.push_back(u.substr(0, i) + c + u.substr(i));
            if (i < (int)u.size()) { string v = u; v[i] = c; next.push_back(v); }
        }
        rep(i, 0, (int)u.size()) next.push_back(u.substr(0, i) + u.substr(i + 1));
        for (auto& v : next) if (!dist.count(v)) dist[v] = dist[u] + 1, q.push(v);
    }
    return -1;
}
void testStrings() {
    rep(iter, 0, 2000) {
        string a = randStr(10, 'c'), b = randStr(10, 'c');
        int best = 0;
        rep(mask, 0, 1 << a.size()) {
            string t;
            rep(i, 0, (int)a.size()) if (mask >> i & 1) t += a[i];
            if (isSubseq(t, b)) best = max(best, (int)t.size());
        }
        string got = lcs(a, b);
        CHECK((int)got.size() == best && isSubseq(got, a) && isSubseq(got, b));
    }
    rep(iter, 0, 1000) {
        string a = randStr(4, 'c'), b = randStr(4, 'c');
        CHECK(editDistance(a, b) == bfsEdit(a, b));
    }
    rep(iter, 0, 2000) {
        string s = randStr(12, 'b');
        vector<string> words(rnd(0, 5));
        for (auto& w : words) { w = randStr(3, 'b'); if (w.empty()) w = "a"; }
        int n = s.size();
        ll cnt = 0;
        set<string> dict(all(words));
        rep(cuts, 0, n ? 1 << (n - 1) : 1) {   // bit i: cut after s[i]
            bool ok = true;
            int start = 0;
            rep(i, 0, n) if (i == n - 1 || (cuts >> i & 1)) {
                if (!dict.count(s.substr(start, i + 1 - start))) ok = false;
                start = i + 1;
            }
            cnt += ok;
        }
        CHECK(wordBreak(s, words) == cnt % M);
    }
    // "a" * 200 with words a, aa: Fibonacci again, far above the mod
    CHECK(wordBreak(string(200, 'a'), {"a", "aa"}) == coinChange({1, 2}, 200).order[200]);
    puts("  LCS, edit distance, word break OK");
}

ll countGrid(const vector<string>& g, int r, int c) {   // every right/down path
    int R = g.size(), C = g[0].size();
    if (r >= R || c >= C || g[r][c] == '#') return 0;
    if (r == R - 1 && c == C - 1) return 1;
    return countGrid(g, r + 1, c) + countGrid(g, r, c + 1);
}
void testGridStairs() {
    rep(iter, 0, 2000) {
        int R = rnd(1, 6), C = rnd(1, 6);
        vector<string> g(R, string(C, '.'));
        rep(r, 0, R) rep(c, 0, C) if (rnd(0, 9) < 2) g[r][c] = '#';
        CHECK(gridPaths(g) == countGrid(g, 0, 0));
    }
    vector<string> open(30, string(30, '.'));   // C(58, 29) paths: needs the mod
    vector<vector<ll>> P(59, vector<ll>(59, 0));   // Pascal's triangle mod M
    rep(i, 0, 59) { P[i][0] = 1; rep(j, 1, i + 1) P[i][j] = (P[i - 1][j - 1] + P[i - 1][j]) % M; }
    CHECK(gridPaths(open) == P[58][29]);
    rep(iter, 0, 300) {
        int n = rnd(0, 14), k = rnd(1, 5);
        vi steps;
        rep(s, 1, k + 1) steps.push_back(s);
        auto dp = stairs(n, k);
        rep(i, 0, n + 1) {
            ll cnt = 0;
            countOrdered(steps, i, cnt);
            CHECK(dp[i] == cnt);
        }
    }
    auto big = stairs(1000, 2);   // Fibonacci again, with the mod
    CHECK(big[1000] == coinChange({1, 2}, 1000).order[1000]);
    puts("  grid paths, stairs with prefix sums OK");
}

void testIntervalsBitmask() {
    rep(iter, 0, 2000) {
        int n = rnd(0, 10);
        vector<array<ll, 3>> jobs(n);   // {e, s, p}
        for (auto& j : jobs) { j[1] = rnd(0, 15); j[0] = j[1] + rnd(1, 6); j[2] = rnd(0, 20); }
        ll best = 0;
        rep(mask, 0, 1 << n) {
            bool ok = true;
            ll sum = 0;
            rep(i, 0, n) if (mask >> i & 1) {
                sum += jobs[i][2];
                rep(j, 0, i) if (mask >> j & 1)
                    if (!(jobs[i][0] <= jobs[j][1] || jobs[j][0] <= jobs[i][1])) ok = false;
            }
            if (ok) best = max(best, sum);
        }
        CHECK(bestProfit(jobs) == best);
    }
    // interval DP: try every order of merging neighbours
    function<ll(vector<ll>)> bruteMerge = [&](vector<ll> a) {
        if (a.size() <= 1) return 0LL;
        ll best = INF;
        rep(i, 0, (int)a.size() - 1) {
            vector<ll> b = a;
            b[i] += b[i + 1];
            b.erase(b.begin() + i + 1);
            best = min(best, b[i] + bruteMerge(b));
        }
        return best;
    };
    rep(iter, 0, 500) {
        vector<ll> a(rnd(1, 7));
        for (ll& x : a) x = rnd(0, 20);
        CHECK(mergePiles(a) == bruteMerge(a));
    }
    rep(iter, 0, 1000) {
        int n = rnd(1, 7);
        vector<vector<ll>> d(n, vector<ll>(n));
        for (auto& row : d) for (ll& x : row) x = rnd(0, 50);
        vi p(n);
        iota(all(p), 0);
        ll best = INF;
        do {   // node 0 first, every order of the rest
            ll len = 0;
            rep(i, 0, n - 1) len += d[p[i]][p[i + 1]];
            best = min(best, len);
        } while (next_permutation(p.begin() + 1, p.end()));
        CHECK(tsp(d) == best);
    }
    rep(iter, 0, 1000) {
        int n = rnd(1, 7);
        vector<vector<ll>> c(n, vector<ll>(n));
        for (auto& row : c) for (ll& x : row) x = rnd(0, 50);
        vi p(n);
        iota(all(p), 0);
        ll best = INF;
        do {   // worker i does job p[i]
            ll sum = 0;
            rep(i, 0, n) sum += c[i][p[i]];
            best = min(best, sum);
        } while (next_permutation(all(p)));
        CHECK(assign(c) == best);
    }
    rep(iter, 0, 2000) {
        int n = rnd(0, 10), k = rnd(0, 11);
        vector<ll> a(n);
        for (ll& x : a) x = rnd(-5, 10);
        ll best = INF;   // bit i of cuts: a cut between a[i] and a[i + 1]
        rep(cuts, 0, n ? 1 << (n - 1) : 1) {
            if (n == 0 ? k != 0 : __builtin_popcount(cuts) != k - 1) continue;
            ll cost = 0, s = 0;
            rep(i, 0, n) {
                s += a[i];
                if (i == n - 1 || (cuts >> i & 1)) cost += s * s, s = 0;
            }
            best = min(best, cost);
        }
        CHECK(splitK(a, k) == best);
    }
    puts("  weighted intervals, interval DP, bitmask TSP, assignment, k parts OK");
}

void testDigits() {
    rep(iter, 0, 3000) {
        ll A = rnd(-1, 3000), B = A + rnd(-1, 3000);
        if (iter % 10 == 0) B = rndll(0, 1000000), A = max(0LL, B - rnd(0, 100000));
        digits::K = rnd(1, 30);
        ll want = 0;
        for (ll x = max(0LL, A); x <= B; x++) {
            int sum = 0;
            for (ll y = x; y > 0; y /= 10) sum += y % 10;
            want += sum % digits::K == 0;
        }
        CHECK(digits::countUpTo(B) - digits::countUpTo(A - 1) == want);
    }
    digits::K = 1;   // every number counts
    CHECK(digits::countUpTo(999999999999999999LL) == 1000000000000000000LL);
    puts("  digit DP OK");
}

int main() {
    testMemoKadane();
    testKnapsack();
    testCoins();
    testLis();
    testStrings();
    testGridStairs();
    testIntervalsBitmask();
    testDigits();
}
