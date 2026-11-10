// mn[j] = min of the window a[j..j+k-1], for every j. O(n).
// The deque holds indices; their values go up from front to back
vector<ll> slidingMin(const vector<ll>& a, int k) {
    vector<ll> mn;
    deque<int> dq;
    rep(i, 0, (int)a.size()) {
        while (!dq.empty() && a[dq.back()] >= a[i]) dq.pop_back();
        dq.push_back(i);
        if (dq.front() <= i - k) dq.pop_front();  // out of window
        if (i >= k - 1) mn.push_back(a[dq.front()]);
    }
    return mn;
}
// window maximum: flip >= to <=
