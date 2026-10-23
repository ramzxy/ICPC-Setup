#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
#define all(x) begin(x), end(x)
#define rep(i, a, b) for (int i = (a); i < (b); i++)
const ll INF = 1e18;

// dbg(x) prints "x = ..." to cerr (also pairs, vectors), but only
// with -DLOCAL (the c command). On the judge it does nothing.
#ifdef LOCAL
template <class A, class B>
ostream& operator<<(ostream& os, const pair<A, B>& p) {
    return os << '(' << p.first << ", " << p.second << ')';
}
template <class T>
ostream& operator<<(ostream& os, const vector<T>& v) {
    os << '{';
    rep(i, 0, (int)v.size()) os << (i ? ", " : "") << v[i];
    return os << '}';
}
#define dbg(x) cerr << #x << " = " << (x) << endl
#else
#define dbg(x)
#endif

int main() {
    cin.tie(0)->sync_with_stdio(0);     // fast input/output
    cout << fixed << setprecision(10);  // 10 decimals
    int n;
    cin >> n;
    cout << n << '\n';   // '\n', not endl: endl is slow
}
