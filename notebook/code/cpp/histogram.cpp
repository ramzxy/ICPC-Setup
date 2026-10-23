// largest rectangle under a histogram with bar heights h. O(n).
// When bar i is popped, it is the lowest bar of a rectangle that
// stretches from just after the new stack top up to i - 1
ll largestRect(const vector<ll>& h) {
    int n = h.size();
    vi st;   // indices, heights going up from bottom to top
    ll best = 0;
    rep(i, 0, n + 1) {
        ll cur = i < n ? h[i] : 0;   // height 0 at the end: pop all
        while (!st.empty() && h[st.back()] >= cur) {
            ll height = h[st.back()];
            st.pop_back();
            int left = st.empty() ? 0 : st.back() + 1;
            best = max(best, height * (i - left));
        }
        st.push_back(i);
    }
    return best;
}
