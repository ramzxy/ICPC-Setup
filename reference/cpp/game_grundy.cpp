// Grundy number of every pile size 0..n for the move set S.
// Several independent piles: xor their Grundy numbers; the player
// to move loses iff the xor is 0. Plain Nim (take any amount):
// the Grundy number of a pile is its size
vi grundy(int n, const vi& S) {
    vi g(n + 1, 0);
    rep(i, 1, n + 1) {
        set<int> reach;   // Grundy numbers you can move to
        for (int s : S) if (s <= i) reach.insert(g[i - s]);
        while (reach.count(g[i])) g[i]++;   // mex: smallest missing
    }
    return g;
}
