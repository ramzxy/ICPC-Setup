// longest strictly increasing subsequence in O(n log n); returns
// its indices. d[k] = smallest last value of an increasing
// subsequence of length k+1 seen so far (d itself is no answer)
vi lis(const vector<ll>& a) {
    vector<ll> d;
    vi at, prv(a.size(), -1);  // at[k]: index of d[k]
    rep(i, 0, (int)a.size()) {
        // non-decreasing (equal allowed): upper_bound here
        int k = lower_bound(all(d), a[i]) - d.begin();
        if (k == (int)d.size()) d.push_back(a[i]), at.push_back(i);
        else d[k] = a[i], at[k] = i;
        if (k > 0) prv[i] = at[k - 1];  // element before a[i]
    }
    vi res;
    for (int i = at.empty() ? -1 : at.back(); i != -1; i = prv[i])
        res.push_back(i);
    reverse(all(res));
    return res;
}
