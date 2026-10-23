// Checks the snippets of sections/greedy.tex against brute force.
#include "check.hpp"
#include "../code/cpp/greedy_intervals.cpp"
#include "../code/cpp/greedy_merge.cpp"
#include "../code/cpp/greedy_sweep.cpp"
#include "../code/cpp/greedy_deadlines.cpp"
#include "../code/cpp/greedy_matching.cpp"

ll exchangeCost(vector<pair<ll, ll>> jobs) {
#include "../code/cpp/greedy_exchange.cpp"
    return cost;
}
ll bestPrice(vector<ll> b) {
    int n = b.size();
#include "../code/cpp/greedy_candidates.cpp"
    return best;
}

using Iv = vector<pair<ll, ll>>;
Iv randIntervals(int n, int maxStart, int minLen, int maxLen) {
    Iv iv(n);
    for (auto& [s, e] : iv) s = rnd(0, maxStart), e = s + rnd(minLen, maxLen);
    return iv;
}

void testExchange() {
    rep(iter, 0, 3000) {
        int n = rnd(0, 7);
        vector<pair<ll, ll>> jobs(n);
        for (auto& [t, w] : jobs) t = rnd(1, 10), w = rnd(1, 10);
        if (iter % 3 == 0)   // many equal ratios
            for (auto& [t, w] : jobs) { ll k = rnd(1, 3); t = k * rnd(1, 2); w = k * rnd(1, 2); }
        ll best = INF;   // try every order
        vector<pair<ll, ll>> p = jobs;
        sort(all(p));
        do {
            ll now = 0, cost = 0;
            for (auto [t, w] : p) now += t, cost += w * now;
            best = min(best, cost);
        } while (next_permutation(all(p)));
        CHECK(exchangeCost(jobs) == best);
    }
    // gluing numbers: sorting strings by a + b > b + a gives the biggest number
    rep(iter, 0, 2000) {
        int n = rnd(1, 6);
        vector<string> v(n);
        for (auto& s : v) s = to_string(rnd(0, rnd(0, 1) ? 9 : 120));
        vector<string> g = v;
        sort(all(g), [](const string& a, const string& b) { return a + b > b + a; });
        string got;
        for (auto& s : g) got += s;
        sort(all(v));
        string best;
        do {
            string c;
            for (auto& s : v) c += s;
            best = max(best, c);   // same length, so string order = number order
        } while (next_permutation(all(v)));
        CHECK(got == best);
    }
    puts("  exchange argument (weighted jobs, gluing numbers) OK");
}

void testIntervals() {
    rep(iter, 0, 3000) {
        int n = rnd(0, 8);
        Iv iv = randIntervals(n, 10, 1, 5);   // [s, e), s < e
        int best = 0;
        rep(mask, 0, 1 << n) {
            bool ok = true;
            rep(i, 0, n) rep(j, 0, i) if ((mask >> i & 1) && (mask >> j & 1)) {
                auto [a, b] = iv[i];
                auto [c, d] = iv[j];
                if (!(b <= c || d <= a)) ok = false;
            }
            if (ok) best = max(best, __builtin_popcount(mask));
        }
        CHECK(maxDisjoint(iv) == best);
    }
    // fewest points hitting every closed [s, e] = maxDisjoint with e + 1
    rep(iter, 0, 1000) {
        int n = rnd(0, 7);
        Iv iv = randIntervals(n, 6, 0, 2);   // inside [0, 8]
        int best = 100;
        rep(pts, 0, 1 << 9) {
            bool ok = true;
            for (auto [s, e] : iv) {
                bool hit = false;
                rep(x, s, e + 1) if (pts >> x & 1) hit = true;
                if (!hit) ok = false;
            }
            if (ok) best = min(best, __builtin_popcount(pts));
        }
        Iv iv1 = iv;
        for (auto& [s, e] : iv1) e++;
        CHECK(maxDisjoint(iv1) == best);
    }
    puts("  interval scheduling, fewest points OK");
}

void testMerge() {
    rep(iter, 0, 3000) {
        int n = rnd(0, 8);
        Iv iv = randIntervals(n, 15, 0, 5);   // closed [s, e], s <= e
        Iv want = iv;   // merge any two that share a point until none do
        bool changed = true;
        while (changed) {
            changed = false;
            rep(i, 0, (int)want.size()) rep(j, 0, i) {
                if (changed) break;
                auto [a, b] = want[i];
                auto [c, d] = want[j];
                if (a <= d && c <= b) {
                    want[j] = {min(a, c), max(b, d)};
                    want.erase(want.begin() + i);
                    changed = true;
                }
            }
        }
        sort(all(want));
        Iv got = mergeIntervals(iv);
        CHECK(got == want);
        ll len = 0, cells = 0;
        for (auto [s, e] : got) len += e - s;
        rep(x, 0, 25) {   // unit pieces [x, x + 1] that are covered
            bool cov = false;
            for (auto [s, e] : iv) if (s <= x && x + 1 <= e) cov = true;
            cells += cov;
        }
        CHECK(len == cells);
    }
    puts("  merging intervals OK");
}

void testSweep() {
    rep(iter, 0, 3000) {
        int n = rnd(0, 8);
        Iv iv = randIntervals(n, 15, 1, 6);   // half-open [s, e)
        int best = 0;
        rep(t, 0, 25) {
            int c = 0;
            for (auto [s, e] : iv) c += s <= t && t < e;
            best = max(best, c);
        }
        CHECK(maxOverlap(iv) == best);
        Iv closed = randIntervals(n, 15, 0, 6);   // closed [s, e]: pass e + 1
        best = 0;
        rep(t, 0, 25) {
            int c = 0;
            for (auto [s, e] : closed) c += s <= t && t <= e;
            best = max(best, c);
        }
        for (auto& [s, e] : closed) e++;
        CHECK(maxOverlap(closed) == best);
    }
    puts("  sweep (most intervals at once) OK");
}

void testDeadlines() {
    rep(iter, 0, 1500) {
        int n = rnd(0, 6);
        vector<pair<ll, ll>> unit(n), dur(n);   // {d, p} and {d, len}
        for (auto& [d, p] : unit) d = rnd(0, 6), p = rnd(0, 20);
        for (auto& [d, l] : dur) d = rnd(0, 15), l = rnd(0, 6);
        ll bestP = 0;
        int bestJ = 0;
        rep(mask, 0, 1 << n) {
            vi ids;
            rep(i, 0, n) if (mask >> i & 1) ids.push_back(i);
            bool okP = false, okJ = false;   // some order of the chosen jobs works
            do {
                bool a = true, b = true;
                ll now = 0;
                rep(k, 0, (int)ids.size()) {
                    if (unit[ids[k]].first < k + 1) a = false;   // slot k + 1
                    now += dur[ids[k]].second;
                    if (now > dur[ids[k]].first) b = false;
                }
                okP |= a, okJ |= b;
            } while (next_permutation(all(ids)));
            ll sum = 0;
            for (int i : ids) sum += unit[i].second;
            if (okP) bestP = max(bestP, sum);
            if (okJ) bestJ = max(bestJ, (int)ids.size());
        }
        CHECK(maxProfit(unit) == bestP);
        CHECK(maxJobs(dur) == bestJ);
    }
    puts("  jobs with deadlines (profit, count) OK");
}

int bruteMatch(const vector<pair<ll, ll>>& people, int i, vector<ll>& items) {
    if (i == (int)people.size()) return 0;
    int best = bruteMatch(people, i + 1, items);   // person i gets nothing
    auto [hi, lo] = people[i];
    rep(j, 0, (int)items.size()) if (items[j] >= lo && items[j] <= hi) {
        ll keep = items[j];
        items[j] = -1;   // used
        best = max(best, 1 + bruteMatch(people, i + 1, items));
        items[j] = keep;
    }
    return best;
}

void testMatching() {
    rep(iter, 0, 3000) {
        int n = rnd(0, 6), m = rnd(0, 6);
        vector<pair<ll, ll>> people(n);
        for (auto& [hi, lo] : people) {
            lo = rnd(0, 10);
            hi = rnd(0, 3) ? lo + rnd(0, 5) : INF;   // sometimes no upper limit
        }
        vector<ll> items(m);
        for (ll& x : items) x = rnd(0, 12);
        CHECK(maxMatch(people, items) == bruteMatch(people, 0, items));
    }
    puts("  matching (smallest item that fits) OK");
}

void testCandidates() {
    rep(iter, 0, 3000) {
        int n = rnd(0, 8);
        vector<ll> b(n);
        for (ll& x : b) x = rnd(0, 20);
        ll best = 0;
        rep(p, 0, 25) {
            ll c = 0;
            for (ll x : b) c += x >= p;
            best = max(best, p * c);
        }
        CHECK(bestPrice(b) == best);
    }
    puts("  try every candidate (best price) OK");
}

int main() {
    testExchange();
    testIntervals();
    testMerge();
    testSweep();
    testDeadlines();
    testMatching();
    testCandidates();
}
