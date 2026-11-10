// bit c of the mask is 1 iff letter c occurs an odd number of
// times. s can be rearranged into a palindrome iff <= 1 bit is set
bool canBePal(const string& s) {
    int mask = 0;
    for (char ch : s) mask ^= 1 << (ch - 'a');
    return __builtin_popcount(mask) <= 1;
}
// number of substrings that can be rearranged into a palindrome.
// s[l..r-1] has mask pre[r] ^ pre[l] (pre = mask of a prefix), so
// count earlier prefixes whose mask differs in at most 1 bit
ll palSubstrings(const string& s) {
    map<int, ll> seen{{0, 1}};   // mask -> how many prefixes
    int mask = 0;
    ll res = 0;
    for (char ch : s) {
        mask ^= 1 << (ch - 'a');
        rep(c, -1, 26) {   // c = -1: all even, else only c odd
            int want = c < 0 ? mask : mask ^ (1 << c);
            if (seen.count(want)) res += seen[want];
        }
        seen[mask]++;
    }
    return res;
}
