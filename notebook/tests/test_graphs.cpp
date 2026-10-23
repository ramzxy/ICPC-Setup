// Checks the snippets of sections/graphs.tex against brute force.
#include "check.hpp"
#include "../code/cpp/graph_types.cpp"
#include "../code/cpp/bfs.cpp"
#include "../code/cpp/grid_bfs.cpp"
#include "../code/cpp/bfs01.cpp"
#include "../code/cpp/dijkstra.cpp"
#include "../code/cpp/bellman_ford.cpp"
#include "../code/cpp/floyd.cpp"
#include "../code/cpp/dfs.cpp"
#include "../code/cpp/topo_sort.cpp"
#include "../code/cpp/dsu.cpp"
#include "../code/cpp/kruskal.cpp"

Graph readGraph() {
#include "../code/cpp/graph_input.cpp"
    return adj;
}

struct RandGraph {
    int n;
    Edges edges;   // {w, u, v}, directed
};
RandGraph randGraph(int wlo, int whi) {
    RandGraph g;
    g.n = rnd(1, 7);
    int m = rnd(0, 15);
    rep(i, 0, m) g.edges.push_back({rnd(wlo, whi), rnd(0, g.n - 1), rnd(0, g.n - 1)});
    return g;
}
vector<vector<ll>> floydOf(const RandGraph& g) {
    vector<vector<ll>> d(g.n, vector<ll>(g.n, INF));
    rep(i, 0, g.n) d[i][i] = 0;
    for (auto [w, u, v] : g.edges) d[u][v] = min(d[u][v], w);
    floyd(d);
    return d;
}
Graph adjOf(const RandGraph& g) {
    Graph a(g.n);
    for (auto [w, u, v] : g.edges) a[u].push_back({v, w});
    return a;
}

void testInput() {
    istringstream in("3 2\n1 2 5\n2 3 7\n");
    auto old = cin.rdbuf(in.rdbuf());
    Graph g = readGraph();
    cin.rdbuf(old);
    CHECK(g.size() == 3);
    CHECK((g[0] == vector<pair<int, ll>>{{1, 5}}));
    CHECK((g[1] == vector<pair<int, ll>>{{0, 5}, {2, 7}}));
    CHECK((g[2] == vector<pair<int, ll>>{{1, 7}}));
    puts("  graph input OK");
}

void testShortestPaths() {
    rep(iter, 0, 3000) {
        RandGraph g = randGraph(0, 9);
        auto d = floydOf(g);
        Graph wadj = adjOf(g);
        int s = rnd(0, g.n - 1);
        vi par;
        auto dist = dijkstra(wadj, s, par);
        vector<ll> bf;
        CHECK(bellmanFord(g.n, g.edges, s, bf));
        rep(v, 0, g.n) {
            CHECK(dist[v] == d[s][v]);
            CHECK(bf[v] == d[s][v]);
            if (dist[v] == INF) continue;
            vi path;   // the path recipe from the snippet's comment
            for (int x = v; x != -1; x = par[x]) path.push_back(x);
            reverse(all(path));
            CHECK(path.front() == s && path.back() == v);
            rep(i, 0, (int)path.size() - 1) {
                bool found = false;
                for (auto [to, w] : wadj[path[i]])
                    if (to == path[i + 1] && dist[path[i]] + w == dist[to]) found = true;
                CHECK(found);
            }
        }
        RandGraph g01 = g;   // 0-1 BFS against Dijkstra
        for (auto& [w, u, v] : g01.edges) w = rnd(0, 1);
        vi tmp;
        CHECK(bfs01(adjOf(g01), s) == dijkstra(adjOf(g01), s, tmp));
        RandGraph g1 = g;    // plain BFS against Dijkstra with weights 1
        vector<vi> uadj(g.n);
        for (auto& [w, u, v] : g1.edges) w = 1, uadj[u].push_back(v);
        auto d1 = dijkstra(adjOf(g1), s, tmp);
        auto b = bfs(uadj, s);
        rep(v, 0, g.n) CHECK(b[v] == (d1[v] == INF ? -1 : d1[v]));
    }
    // negative weights: Bellman-Ford must find exactly the negative cycles reachable from s
    rep(iter, 0, 3000) {
        RandGraph g = randGraph(-5, 9);
        auto d = floydOf(g);
        int s = rnd(0, g.n - 1);
        bool negCycle = false;
        rep(i, 0, g.n) if (d[s][i] < INF && d[i][i] < 0) negCycle = true;
        vector<ll> bf;
        bool ok = bellmanFord(g.n, g.edges, s, bf);
        CHECK(ok == !negCycle);
        if (ok) rep(v, 0, g.n) CHECK(bf[v] == d[s][v]);
    }
    puts("  BFS, 0-1 BFS, Dijkstra + path, Bellman-Ford, Floyd OK");
}

void testGrid() {
    rep(iter, 0, 2000) {
        int R = rnd(1, 6), C = rnd(1, 6);
        vector<string> g(R, string(C, '.'));
        rep(r, 0, R) rep(c, 0, C) if (rnd(0, 9) < 3) g[r][c] = '#';
        int sr = rnd(0, R - 1), sc = rnd(0, C - 1);
        g[sr][sc] = '.';
        vector<vi> adjG(R * C);   // same grid as an explicit graph
        rep(r, 0, R) rep(c, 0, C) rep(d, 0, 4) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr >= 0 && nr < R && nc >= 0 && nc < C && g[r][c] == '.' && g[nr][nc] == '.')
                adjG[r * C + c].push_back(nr * C + nc);
        }
        auto ref = bfs(adjG, sr * C + sc);
        auto got = gridBfs(g, sr, sc);
        rep(r, 0, R) rep(c, 0, C) CHECK(got[r][c] == ref[r * C + c]);
    }
    puts("  grid BFS OK");
}

void testComponentsTopoMst() {
    rep(iter, 0, 3000) {
        int n = rnd(1, 7), m = rnd(0, 10);
        Edges es;
        rep(i, 0, m) es.push_back({rnd(0, 20), rnd(0, n - 1), rnd(0, n - 1)});
        vector<vector<bool>> reach(n, vector<bool>(n, false));   // undirected
        rep(i, 0, n) reach[i][i] = true;
        for (auto [w, u, v] : es) reach[u][v] = reach[v][u] = true;
        rep(k, 0, n) rep(i, 0, n) rep(j, 0, n) if (reach[i][k] && reach[k][j]) reach[i][j] = true;
        adj.assign(n, {});   // globals adj, comp from the DFS snippet
        for (auto [w, u, v] : es) adj[u].push_back(v), adj[v].push_back(u);
        comp.assign(n, -1);
        int k = 0;
        rep(u, 0, n) if (comp[u] == -1) dfs(u, k++);
        DSU dsu(n);
        for (auto [w, u, v] : es) dsu.unite(u, v);
        rep(i, 0, n) {
            int cnt = 0;
            rep(j, 0, n) {
                CHECK((comp[i] == comp[j]) == reach[i][j]);
                CHECK((dsu.find(i) == dsu.find(j)) == reach[i][j]);
                cnt += reach[i][j];
            }
            CHECK(dsu.sz[dsu.find(i)] == cnt);
        }
        ll best = -1;   // MST: try every subset of n - 1 edges
        rep(mask, 0, 1 << m) if (__builtin_popcount(mask) == n - 1) {
            DSU t(n);
            bool tree = true;
            ll w = 0;
            rep(i, 0, m) if (mask >> i & 1) {
                auto [ew, u, v] = es[i];
                if (!t.unite(u, v)) tree = false;
                w += ew;
            }
            if (tree && (best == -1 || w < best)) best = w;
        }
        CHECK(kruskal(n, es) == best);
        vector<vi> dadj(n);   // topological sort on the directed version
        vector<vector<bool>> dreach(n, vector<bool>(n, false));
        for (auto [w, u, v] : es) dadj[u].push_back(v), dreach[u][v] = true;
        rep(kk, 0, n) rep(i, 0, n) rep(j, 0, n) if (dreach[i][kk] && dreach[kk][j]) dreach[i][j] = true;
        bool cycle = false;
        rep(i, 0, n) if (dreach[i][i]) cycle = true;
        vi order = topoSort(dadj);
        CHECK(((int)order.size() == n) == !cycle);
        if (!cycle) {
            vi pos(n, -1);
            rep(i, 0, n) pos[order[i]] = i;
            rep(i, 0, n) CHECK(pos[i] != -1);
            for (auto [w, u, v] : es) CHECK(pos[u] < pos[v]);
        }
    }
    puts("  DFS components, DSU, Kruskal, topological sort OK");
}

int main() {
    testInput();
    testShortestPaths();
    testGrid();
    testComponentsTopoMst();
}
