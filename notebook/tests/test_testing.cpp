// Checks the snippets of sections/testing.tex against brute force.
// code/sh/stress.sh is a shell script: run it by hand on a small example.
#define LOCAL   // so the template's dbg() is switched on
#include "check.hpp"
#include "../code/cpp/bf_backtrack.cpp"
#include "../code/cpp/bf_interactive.cpp"

// the generator has its own main: only check that it compiles
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wreturn-type"
#define main genMain
#include "../code/cpp/bf_gen.cpp"
#undef main
#pragma GCC diagnostic pop

bool subsetSum(int n, const vector<ll>& a, ll S) {
#include "../code/cpp/bf_subsets.cpp"
    return found;
}
ll shortestRoute(int n, const vector<vector<ll>>& d) {
#include "../code/cpp/bf_perms.cpp"
    return best;
}

bool canReach(const vector<ll>& a, int i, ll S) {   // take a[i] or not
    if (i == (int)a.size()) return S == 0;
    return canReach(a, i + 1, S) || canReach(a, i + 1, S - a[i]);
}

void testSubsetsPerms() {
    rep(iter, 0, 3000) {
        int n = rnd(0, 10);
        vector<ll> a(n);
        for (ll& x : a) x = rnd(-5, 12);
        ll S = rnd(-10, 40);
        CHECK(subsetSum(n, a, S) == canReach(a, 0, S));
    }
    rep(iter, 0, 500) {   // route through all cities: bitmask DP
        int n = rnd(1, 7);
        vector<vector<ll>> d(n, vector<ll>(n));
        for (auto& row : d) for (ll& x : row) x = rnd(0, 50);
        vector<vector<ll>> dp(1 << n, vector<ll>(n, INF));
        rep(v, 0, n) dp[1 << v][v] = 0;
        rep(mask, 1, 1 << n) rep(v, 0, n) if (dp[mask][v] < INF)
            rep(w, 0, n) if (!(mask >> w & 1))
                dp[mask | 1 << w][w] = min(dp[mask | 1 << w][w], dp[mask][v] + d[v][w]);
        ll want = *min_element(all(dp[(1 << n) - 1]));
        CHECK(shortestRoute(n, d) == want);
    }
    puts("  all subsets, all orders OK");
}

void testBacktrack() {
    rep(iter, 0, 3000) {
        int n = rnd(0, 7), k = rnd(1, 4);
        vector<ll> jobs(n);
        for (ll& x : jobs) x = rnd(0, 3) ? rnd(1, 20) : 0;
        ll want = INF;   // brute force: all k^n ways
        int ways = 1;
        rep(i, 0, n) ways *= k;
        rep(w, 0, ways) {
            vector<ll> l(k, 0);
            int x = w;
            rep(i, 0, n) l[x % k] += jobs[i], x /= k;
            want = min(want, *max_element(all(l)));
        }
        job = jobs;
        if (iter % 2) sort(job.rbegin(), job.rend());   // the usage line
        load.assign(k, 0);
        best = INF;
        place(0, 0);
        CHECK(best == want);
        CHECK(*max_element(all(load)) == 0);   // all undone
    }
    puts("  backtracking OK");
}

// Plays the judge of bf_interactive.cpp. Output only reaches the judge
// when it is flushed, so a missing endl makes the program get stuck.
struct FakeJudge : streambuf {
    ll n, secret, answer = -1;
    int questions = 0;
    bool sentN = false, stuck = false;
    string unflushed, flushed, reply;
    FakeJudge(ll n_, ll s) : n(n_), secret(s) {}
    int overflow(int c) override {   // the program prints c
        if (c != EOF) unflushed += char(c);
        return c;
    }
    int sync() override {   // the program flushes
        flushed += unflushed;
        unflushed.clear();
        return 0;
    }
    void readLines() {
        size_t nl;
        while ((nl = flushed.find('\n')) != string::npos) {
            istringstream line(flushed.substr(0, nl));
            flushed.erase(0, nl + 1);
            char type;
            ll y;
            CHECK(line >> type >> y);
            if (type == '?') {
                questions++;
                reply += secret >= y ? "1\n" : "0\n";
            } else {
                CHECK(type == '!' && answer == -1);
                answer = y;
            }
        }
    }
    int underflow() override {   // the program waits for the judge
        reply.clear();
        if (!sentN) reply = to_string(n) + "\n", sentN = true;
        readLines();
        if (reply.empty()) { stuck = true; return traits_type::eof(); }
        setg(&reply[0], &reply[0], &reply[0] + reply.size());
        return traits_type::to_int_type(reply[0]);
    }
};

void runInteractive(ll n, ll secret) {
    FakeJudge judge(n, secret);
    auto oldIn = cin.rdbuf(&judge);
    auto oldOut = cout.rdbuf(&judge);
    auto oldTie = cin.tie(nullptr);   // like the template: no auto flush
    solve();
    bool ok = bool(cin);
    cin.rdbuf(oldIn), cout.rdbuf(oldOut), cin.tie(oldTie);
    cin.clear();
    judge.readLines();
    CHECK(ok && !judge.stuck && judge.unflushed.empty());
    CHECK(judge.answer == secret);
    int limit = 0;   // binary search: at most ceil(log2 n) questions
    while ((1LL << limit) < n) limit++;
    CHECK(judge.questions <= limit);
}

void testInteractive() {
    rep(n, 1, 70) rep(x, 1, n + 1) runInteractive(n, x);
    rep(iter, 0, 300) {
        ll n = rndll(1, (ll)1e18);
        runInteractive(n, rndll(1, n));
        runInteractive(n, iter % 2 ? 1 : n);
    }
    puts("  interactive skeleton OK");
}

void testDebug() {
    stringstream out;
    auto old = cerr.rdbuf(out.rdbuf());
    int n = 3;
    vi a = {4, 5, 6}, e;
    vector<pii> ps = {{1, 2}, {3, 4}};
    vector<vi> g = {{1}, {}, {2, 3}};
    string s = "hi";
    dbg(n); dbg(a); dbg(a[1] + 1); dbg(e); dbg(ps); dbg(g); dbg(s);
    cerr.rdbuf(old);
    CHECK(out.str() == "n = 3\na = {4, 5, 6}\na[1] + 1 = 6\ne = {}\n"
                       "ps = {(1, 2), (3, 4)}\ng = {{1}, {}, {2, 3}}\ns = hi\n");
    puts("  debug output OK");
}

int main() {
    testDebug();
    testSubsetsPerms();
    testBacktrack();
    testInteractive();
}
