// nxt[i] = first index j > i with a[j] > a[i], or n if none.
// The stack holds indices still waiting for their answer; their
// values go down from bottom to top. O(n) in total
vi nextGreater(const vector<ll>& a) {
    int n = a.size();
    vi nxt(n, n), st;
    rep(i, 0, n) {
        while (!st.empty() && a[st.back()] < a[i]) {
            nxt[st.back()] = i;  // a[i] answers st.back()
            st.pop_back();
        }
        st.push_back(i);
    }
    return nxt;
}
