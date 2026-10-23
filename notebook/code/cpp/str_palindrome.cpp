bool isPal(const string& s) { return equal(all(s), s.rbegin()); }
// longest palindromic substring in O(n^2): grow around every
// centre (a letter for odd lengths, a gap for even). {start, len}
pii longestPal(const string& s) {
    int n = s.size(), start = 0, best = 0;
    rep(c, 0, 2 * n - 1) {   // c even: centre s[c/2]; odd: a gap
        int l = c / 2, r = l + c % 2;
        while (l >= 0 && r < n && s[l] == s[r]) l--, r++;
        if (r - l - 1 > best) start = l + 1, best = r - l - 1;
    }
    return {start, best};   // the text: s.substr(start, best)
}
