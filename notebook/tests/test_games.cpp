// Checks the snippets of sections/games.tex against a brute-force game search.
#include "check.hpp"
#include "../code/cpp/game_win.cpp"
#include "../code/cpp/game_grundy.cpp"

vector<vector<ll>> endsTable(const vector<ll>& a) {
    int n = a.size();
#include "../code/cpp/game_ends.cpp"
    return d;
}

// does the player to move win with these piles? (searches every move)
map<vi, bool> memoWin;
bool bruteWin(vi p, const vi& S) {
    sort(all(p));
    auto it = memoWin.find(p);
    if (it != memoWin.end()) return it->second;
    bool win = false;
    rep(i, 0, (int)p.size()) for (int s : S) if (s <= p[i]) {
        vi q = p;
        q[i] -= s;
        if (!bruteWin(q, S)) win = true;
    }
    return memoWin[p] = win;
}

// the most the player to move can collect from a[l..r]
ll bestTotal(const vector<ll>& a, int l, int r) {
    if (l > r) return 0;
    ll sum = 0;
    rep(i, l, r + 1) sum += a[i];
    return sum - min(bestTotal(a, l + 1, r), bestTotal(a, l, r - 1));
}

int main() {
    rep(iter, 0, 300) {
        vi S;
        rep(v, 1, 6) if (rnd(0, 1)) S.push_back(v);
        if (S.empty()) S.push_back(rnd(1, 5));
        int n = 30;
        auto win = winning(n, S);
        auto g = grundy(n, S);
        memoWin.clear();
        rep(i, 0, n + 1) {
            CHECK(win[i] == (g[i] != 0));
            CHECK(bruteWin({i}, S) == win[i]);
        }
        rep(t, 0, 20) {   // several piles: the xor rule
            vi p(rnd(1, 3));
            int x = 0;
            for (int& v : p) v = rnd(0, 8), x ^= g[v];
            CHECK(bruteWin(p, S) == (x != 0));
        }
    }
    vi nim(20);
    iota(nim.begin(), nim.end(), 1);   // plain Nim: take any amount
    auto g = grundy(20, nim);
    rep(i, 0, 21) CHECK(g[i] == i);
    puts("  winning positions, Grundy numbers, Nim OK");
    rep(iter, 0, 2000) {
        int n = rnd(1, 10);
        vector<ll> a(n);
        ll sum = 0;
        for (ll& v : a) v = rnd(0, 9), sum += v;
        auto d = endsTable(a);
        CHECK((sum + d[0][n - 1]) % 2 == 0);
        CHECK((sum + d[0][n - 1]) / 2 == bestTotal(a, 0, n - 1));
    }
    puts("  taking from both ends OK");
}
