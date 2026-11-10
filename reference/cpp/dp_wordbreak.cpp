// ways to cut s into words from the list (mod 1e9+7).
// ways[i] = ways to cut the first i characters; the last piece
// is s[i-len .. i-1]. O(|s| * L) lookups, L = longest word
ll wordBreak(const string& s, const vector<string>& words) {
    const ll MOD = 1e9 + 7;
    set<string> dict(all(words));
    int n = s.size(), L = 0;
    for (auto& w : words) L = max(L, (int)w.size());
    vector<ll> ways(n + 1, 0);
    ways[0] = 1;  // the empty prefix: one way
    rep(i, 1, n + 1)
        for (int len = 1; len <= min(i, L); len++)
            if (dict.count(s.substr(i - len, len)))
                ways[i] = (ways[i] + ways[i - len]) % MOD;
    return ways[n];
}
