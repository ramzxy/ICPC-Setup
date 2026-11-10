// pi[i] = length of the longest prefix of s[0..i] that is also a
// suffix of s[0..i] (not the whole s[0..i] itself). O(n)
vi prefixFunction(const string& s) {
    int n = s.size();
    vi pi(n, 0);
    rep(i, 1, n) {
        int k = pi[i - 1];
        while (k > 0 && s[i] != s[k]) k = pi[k - 1];  // try shorter
        if (s[i] == s[k]) k++;
        pi[i] = k;
    }
    return pi;
}
// start of every occurrence of pat in txt, overlaps too. O(n + m)
// '#' must be a character that is in neither string
vi findAll(const string& pat, const string& txt) {
    int m = pat.size();
    vi pi = prefixFunction(pat + '#' + txt), res;
    rep(i, 0, (int)pi.size())
        if (pi[i] == m) res.push_back(i - 2 * m);
    return res;
}
// period: the smallest p with s[i] == s[i + p] for all i is
// p = n - pi[n - 1]. s is s[0..p-1] repeated iff n % p == 0
