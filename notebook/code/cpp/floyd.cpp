// in:  d[i][j] = weight of edge i -> j, INF if there is none,
//      d[i][i] = 0
// out: d[i][j] = shortest distance from i to j.
//      d[i][i] < 0 means i is on a negative cycle
void floyd(vector<vector<ll>>& d) {
    int n = d.size();
    rep(k, 0, n)  // k = the node in the middle: OUTER loop!
        rep(i, 0, n) rep(j, 0, n)
            if (d[i][k] < INF && d[k][j] < INF)
                d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
}
