// every node v has one next node nxt[v]. jump(v, k) = where you
// are after k steps from v, for k up to 1e18. Memory: 60*n ints.
struct Jump {
    vector<vi> up;   // up[j][v] = node 2^j steps after v
    Jump(const vi& nxt) : up(60, nxt) {
        rep(j, 1, 60) rep(v, 0, (int)nxt.size())
            up[j][v] = up[j - 1][up[j - 1][v]];
    }
    int jump(int v, ll k) {
        rep(j, 0, 60) if (k >> j & 1) v = up[j][v];
        return v;
    }
};
