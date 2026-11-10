// memoized recursion. Example: from stone i you may jump to
// i+1 .. i+k, paying |h[i] - h[j]|. Cheapest way from stone 0 to
// stone n-1. memo[i] == -1 means "not computed yet"
int n, k;
vector<ll> h, memo;
ll solve(int i) {  // cheapest cost from stone i to the last one
    if (i == n - 1) return 0;           // base case
    ll& res = memo[i];  // a reference: setting res fills memo[i]
    if (res != -1) return res;          // computed before
    res = INF;
    rep(j, i + 1, min(n, i + k + 1))
        res = min(res, abs(h[i] - h[j]) + solve(j));
    return res;
}
// main: read n, k, h;  memo.assign(n, -1);  cout << solve(0);
