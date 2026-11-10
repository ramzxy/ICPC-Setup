// interactive example: find a secret x in [1, n]. To "? y" the
// judge answers 1 if x >= y, else 0. Give the answer as "! x".
int ask(ll y) {
    cout << "? " << y << endl;   // endl flushes: without it the
    int reply;                   // judge never sees the question
    cin >> reply;
    return reply;
}
void solve() {   // call this from main
    ll n;
    cin >> n;
    ll lo = 1, hi = n;   // x is in [lo, hi]
    while (lo < hi) {
        ll mid = lo + (hi - lo + 1) / 2;
        if (ask(mid)) lo = mid;   // x >= mid
        else hi = mid - 1;
    }
    cout << "! " << lo << endl;
}
