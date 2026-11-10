// how many x in [0, N] have a digit sum divisible by K (K <= 100)?
// go(pos, rem, tight): the digits before pos are chosen, rem =
// their sum mod K, tight = they equal N's first digits
string D;  // the digits of N
int K;
ll memo[20][100][2];
ll go(int pos, int rem, bool tight) {
    if (pos == (int)D.size()) return rem == 0;
    ll& res = memo[pos][rem][tight];
    if (res != -1) return res;
    res = 0;
    int hi = tight ? D[pos] - '0' : 9;  // tight: up to N's digit
    rep(d, 0, hi + 1)
        res += go(pos + 1, (rem + d) % K, tight && d == hi);
    return res;
}
// count in [A, B]: countUpTo(B) - countUpTo(A - 1)
ll countUpTo(ll N) {
    if (N < 0) return 0;
    D = to_string(N);
    memset(memo, -1, sizeof memo);
    return go(0, 0, true);
}
