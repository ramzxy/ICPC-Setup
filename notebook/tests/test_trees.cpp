// Checks the snippets of sections/trees.tex against brute force.
#include "check.hpp"
#include "../code/cpp/bfs.cpp"
#include "../code/cpp/topo_sort.cpp"
#include "../code/cpp/tree_root.cpp"
#include "../code/cpp/tree_dfs.cpp"
#include "../code/cpp/tree_dp.cpp"
#include "../code/cpp/tree_diameter.cpp"
#include "../code/cpp/tree_lca.cpp"
#include "../code/cpp/graph2_jump.cpp"
#include "../code/cpp/tree_reroot.cpp"
#include "../code/cpp/graph2_scc.cpp"
#include "../code/cpp/graph2_euler.cpp"
#include "../code/cpp/graph2_bipartite.cpp"
#include "../code/cpp/graph2_matching.cpp"

vi sizesOf(int n, const vi& order, const vi& par) {
#include "../code/cpp/tree_size.cpp"
    return sz;
}

// random tree on n nodes with random labels and shuffled neighbours.
// shape 0: random parent, 1: path, 2: star, 3: deep (parent close by)
vector<vi> randTree(int n, int shape = -1) {
    if (shape == -1) shape = rnd(0, 3);
    vi perm(n);
    iota(all(perm), 0);
    shuffle(all(perm), rng);
    vector<vi> adj(n);
    rep(v, 1, n) {
        int p = shape == 0 ? rnd(0, v - 1) : shape == 1 ? v - 1
              : shape == 2 ? 0 : rnd(max(0, v - 3), v - 1);
        adj[perm[p]].push_back(perm[v]);
        adj[perm[v]].push_back(perm[p]);
    }
    for (auto& l : adj) shuffle(all(l), rng);
    return adj;
}
vector<vi> allPairs(const vector<vi>& adj) {
    vector<vi> d;
    rep(s, 0, (int)adj.size()) d.push_back(bfs(adj, s));
    return d;
}

void testRootAndSizes() {
    rep(iter, 0, 3000) {
        int n = rnd(1, 12), r = rnd(0, n - 1);
        auto adj = randTree(n);
        vi par, dep;
        vi order = rootTree(adj, r, par, dep);
        CHECK((int)order.size() == n && order[0] == r);
        vi pos(n, -1);
        rep(i, 0, n) { CHECK(pos[order[i]] == -1); pos[order[i]] = i; }
        vi d = bfs(adj, r);
        rep(v, 0, n) {
            CHECK(dep[v] == d[v]);
            if (v == r) { CHECK(par[v] == -1); continue; }
            CHECK(count(all(adj[v]), par[v]) == 1);
            CHECK(pos[par[v]] < pos[v] && dep[par[v]] == dep[v] - 1);
        }
        vi sz = sizesOf(n, order, par);
        rep(v, 0, n) {   // count the nodes that have v on their way up
            int cnt = 0;
            rep(u, 0, n) for (int x = u; x != -1; x = par[x]) cnt += x == v;
            CHECK(sz[v] == cnt);
        }
    }
    // a path of 2e5 nodes must not crash (no recursion)
    int n = 200000;
    auto adj = randTree(n, 1);
    vi par, dep;
    vi order = rootTree(adj, 0, par, dep);
    vi sz = sizesOf(n, order, par);
    CHECK(sz[0] == n && *max_element(all(dep)) <= n - 1);
    puts("  rooting, subtree sizes OK");
}

// plain recursive DFS that tries neighbours in the same order
void refDfs(const vector<vi>& adj, int u, int p, vi& tin, int& timer) {
    tin[u] = timer++;
    for (int v : adj[u]) if (v != p) refDfs(adj, v, u, tin, timer);
}

void testDfsOrder() {
    rep(iter, 0, 3000) {
        int n = rnd(1, 12), r = rnd(0, n - 1);
        auto adj = randTree(n);
        vi tin, tout, ref(n, -1), par, dep;
        dfsOrder(adj, r, tin, tout);
        int timer = 0;
        refDfs(adj, r, -1, ref, timer);
        CHECK(tin == ref);
        rootTree(adj, r, par, dep);
        rep(u, 0, n) rep(v, 0, n) {
            bool below = false;   // is u in the subtree of v?
            for (int x = u; x != -1; x = par[x]) below |= x == v;
            CHECK(below == (tin[v] <= tin[u] && tin[u] < tout[v]));
        }
    }
    int n = 200000;   // long path: no recursion
    auto adj = randTree(n, 1);
    vi tin, tout;
    dfsOrder(adj, 0, tin, tout);
    CHECK(tout[0] == n && *max_element(all(tin)) == n - 1);
    puts("  DFS order (tin / tout) OK");
}

void testTreeDP() {
    rep(iter, 0, 3000) {
        int n = rnd(1, 12);
        auto adj = randTree(n);
        vector<ll> w(n);
        for (ll& x : w) x = rnd(-3, 9);
        ll best = 0;   // brute force: every set without two neighbours
        rep(mask, 0, 1 << n) {
            bool ok = true;
            ll s = 0;
            rep(u, 0, n) if (mask >> u & 1) {
                s += w[u];
                for (int v : adj[u]) if (mask >> v & 1) ok = false;
            }
            if (ok) best = max(best, s);
        }
        CHECK(maxIndependentSet(adj, w) == best);
    }
    puts("  tree DP (max independent set) OK");
}

void testDiameterReroot() {
    rep(iter, 0, 3000) {
        int n = rnd(1, 14);
        auto adj = randTree(n);
        auto d = allPairs(adj);
        int best = 0;
        rep(u, 0, n) rep(v, 0, n) best = max(best, d[u][v]);
        int a = -1, b = -1;
        CHECK(diameter(adj, a, b) == best);
        CHECK(d[a][b] == best);
        auto ans = sumOfDistances(adj);
        CHECK((int)ans.size() == n);
        rep(u, 0, n) {
            ll s = 0;
            rep(v, 0, n) s += d[u][v];
            CHECK(ans[u] == s);
        }
    }
    int n = 200000;   // long path: no recursion, sums need long long
    auto adj = randTree(n, 1);
    auto ans = sumOfDistances(adj);
    CHECK(*max_element(all(ans)) == (ll)n * (n - 1) / 2);
    int a, b;
    CHECK(diameter(adj, a, b) == n - 1);
    puts("  diameter, rerooting (sum of distances) OK");
}

void testLCA() {
    rep(iter, 0, 2000) {
        int n = rnd(1, 20), r = rnd(0, n - 1);
        auto adj = randTree(n);
        LCA t(adj, r);
        vi par, dep;
        rootTree(adj, r, par, dep);
        auto d = allPairs(adj);
        rep(a, 0, n) rep(b, 0, n) {
            set<int> above;   // brute force: walk up from a, then from b
            for (int x = a; x != -1; x = par[x]) above.insert(x);
            int x = b;
            while (!above.count(x)) x = par[x];
            CHECK(t.lca(a, b) == x);
            CHECK(t.dist(a, b) == d[a][b]);
        }
        rep(v, 0, n) rep(k, 0, n + 2) {
            int x = v;
            rep(i, 0, k) if (x != -1) x = par[x];
            CHECK(t.kth(v, k) == x);
        }
    }
    int n = 200000;   // deep path, rooted at one of its ends
    auto adj = randTree(n, 1);
    int e = 0;
    while (adj[e].size() != 1) e++;
    LCA t(adj, e);
    vi par, dep;
    vi order = rootTree(adj, e, par, dep);
    int deepest = order.back();
    CHECK(t.dist(e, deepest) == n - 1);
    CHECK(t.lca(deepest, order[n / 2]) == order[n / 2]);
    CHECK(t.kth(deepest, n - 1) == e && t.kth(deepest, n) == -1);
    puts("  LCA, k-th ancestor, distance OK");
}

void testJump() {
    rep(iter, 0, 2000) {
        int n = rnd(1, 9);
        vi nxt(n);
        for (int& x : nxt) x = rnd(0, n - 1);
        Jump J(nxt);
        rep(v, 0, n) {
            int x = v;
            rep(k, 0, 70) { CHECK(J.jump(v, k) == x); x = nxt[x]; }
            // big k: walk until a node repeats, then use the cycle
            ll k = iter % 2 ? rndll(0, (ll)1e18) : (ll)1e18 - rnd(0, 5);
            vi first(n, -1), path;
            x = v;
            while (first[x] == -1) { first[x] = path.size(); path.push_back(x); x = nxt[x]; }
            int start = first[x], len = path.size() - start;
            int want = k < (ll)path.size() ? path[k] : path[start + (k - start) % len];
            CHECK(J.jump(v, k) == want);
        }
    }
    puts("  jumping k steps (binary lifting) OK");
}

void testSCC() {
    rep(iter, 0, 3000) {
        int n = rnd(1, 8), m = rnd(0, 16);
        vector<vi> adj(n);
        vector<vector<bool>> reach(n, vector<bool>(n, false));
        rep(i, 0, n) reach[i][i] = true;
        set<pii> edges;
        rep(i, 0, m) {
            int u = rnd(0, n - 1), v = rnd(0, n - 1);
            adj[u].push_back(v);
            reach[u][v] = true;
            edges.insert({u, v});
        }
        rep(k, 0, n) rep(i, 0, n) rep(j, 0, n)
            if (reach[i][k] && reach[k][j]) reach[i][j] = true;
        SCC s(adj);
        set<int> ids;
        rep(u, 0, n) {
            CHECK(s.scc[u] >= 0 && s.scc[u] < s.k);
            ids.insert(s.scc[u]);
            rep(v, 0, n) CHECK((s.scc[u] == s.scc[v]) == (reach[u][v] && reach[v][u]));
        }
        CHECK((int)ids.size() == s.k);
        for (auto [u, v] : edges) CHECK(s.scc[u] <= s.scc[v]);
        auto dag = s.condense();
        CHECK((int)dag.size() == s.k);
        multiset<pii> want, got;
        rep(u, 0, n) for (int v : adj[u])
            if (s.scc[u] != s.scc[v]) want.insert({s.scc[u], s.scc[v]});
        rep(a, 0, s.k) for (int b : dag[a]) got.insert({a, b});
        CHECK(want == got);
        CHECK((int)topoSort(dag).size() == s.k);   // no cycles
    }
    puts("  SCC, condensation OK");
}

// brute force: can we walk from u and use every unused edge once?
bool walkAll(int u, int usedMask, const vector<pii>& es, bool directed) {
    int m = es.size();
    if (usedMask == (1 << m) - 1) return true;
    rep(i, 0, m) if (!(usedMask >> i & 1)) {
        auto [a, b] = es[i];
        if (a == u && walkAll(b, usedMask | 1 << i, es, directed)) return true;
        if (!directed && b == u && walkAll(a, usedMask | 1 << i, es, directed)) return true;
    }
    return false;
}

void testEuler() {
    int found = 0;
    rep(iter, 0, 4000) {
        int n = rnd(1, 5), m = rnd(0, 7);
        bool directed = iter % 2;
        vector<pii> es;
        rep(i, 0, m) es.push_back({rnd(0, n - 1), rnd(0, n - 1)});
        bool exists = false;
        rep(u, 0, n) if (walkAll(u, 0, es, directed)) exists = true;
        vi path = eulerPath(n, es, directed);
        CHECK(!path.empty() == exists);
        if (path.empty()) continue;
        found++;
        CHECK((int)path.size() == m + 1);
        multiset<pii> left;   // every edge exactly once
        for (auto [a, b] : es) left.insert(directed ? pii(a, b) : pii(min(a, b), max(a, b)));
        rep(i, 0, m) {
            int a = path[i], b = path[i + 1];
            pii e = directed ? pii(a, b) : pii(min(a, b), max(a, b));
            CHECK(left.count(e));
            left.erase(left.find(e));
        }
    }
    CHECK(found > 1000);
    puts("  Euler path (directed and undirected) OK");
}

void testBipartite() {
    rep(iter, 0, 3000) {
        int n = rnd(1, 9), m = rnd(0, 10);
        vector<vi> adj(n);
        vector<pii> es;
        rep(i, 0, m) {
            int u = rnd(0, n - 1), v = rnd(0, n - 1);
            if (iter % 3 == 0 && u == v) continue;
            adj[u].push_back(v), adj[v].push_back(u);
            es.push_back({u, v});
        }
        bool possible = false;   // brute force: try all 2^n colourings
        rep(mask, 0, 1 << n) {
            bool ok = true;
            for (auto [u, v] : es) if ((mask >> u & 1) == (mask >> v & 1)) ok = false;
            if (ok) possible = true;
        }
        vi col;
        CHECK(bipartite(adj, col) == possible);
        if (possible) {
            rep(v, 0, n) CHECK(col[v] == 0 || col[v] == 1);
            for (auto [u, v] : es) CHECK(col[u] != col[v]);
        }
    }
    rep(iter, 0, 200) {   // trees are always bipartite
        auto adj = randTree(rnd(1, 30));
        vi col;
        CHECK(bipartite(adj, col));
    }
    puts("  bipartite check OK");
}

void testMatching() {
    rep(iter, 0, 3000) {
        int nl = rnd(0, 5), nr = rnd(1, 5), m = nl ? rnd(0, 11) : 0;
        vector<vi> adj(nl);
        vector<pii> es;
        set<pii> has;
        rep(i, 0, m) {
            int l = rnd(0, nl - 1), r = rnd(0, nr - 1);
            adj[l].push_back(r);
            es.push_back({l, r});
            has.insert({l, r});
        }
        int best = 0;   // brute force: every set of edges without shared ends
        rep(mask, 0, 1 << m) {
            vector<bool> ul(nl), ur(nr);
            bool ok = true;
            rep(i, 0, m) if (mask >> i & 1) {
                auto [l, r] = es[i];
                if (ul[l] || ur[r]) ok = false;
                ul[l] = ur[r] = true;
            }
            if (ok) best = max(best, __builtin_popcount(mask));
        }
        Matching M(adj, nr);
        CHECK(M.solve() == best);
        set<int> lefts;
        int pairs = 0;
        rep(r, 0, nr) if (M.mt[r] != -1) {
            CHECK(has.count({M.mt[r], r}));
            lefts.insert(M.mt[r]);
            pairs++;
        }
        CHECK(pairs == best && (int)lefts.size() == best);
    }
    puts("  bipartite matching OK");
}

int main() {
    testRootAndSizes();
    testDfsOrder();
    testTreeDP();
    testDiameterReroot();
    testLCA();
    testJump();
    testSCC();
    testEuler();
    testBipartite();
    testMatching();
}
