// non-negative big numbers as decimal strings without leading
// zeros, e.g. "12345". All O(length), multiply O(|a| * |b|)
bool lessBig(const string& a, const string& b) {   // a < b
    if (a.size() != b.size()) return a.size() < b.size();
    return a < b;
}
string addBig(const string& a, const string& b) {
    string r;
    int i = a.size() - 1, j = b.size() - 1, carry = 0;
    while (i >= 0 || j >= 0 || carry) {
        if (i >= 0) carry += a[i--] - '0';
        if (j >= 0) carry += b[j--] - '0';
        r += char('0' + carry % 10);
        carry /= 10;
    }
    reverse(all(r));
    return r;
}
string subBig(const string& a, const string& b) {   // needs a >= b
    string r;
    int i = a.size() - 1, j = b.size() - 1, borrow = 0;
    while (i >= 0) {
        int s = a[i--] - '0' - borrow - (j >= 0 ? b[j--] - '0' : 0);
        borrow = s < 0;
        r += char('0' + (s + 10) % 10);
    }
    while (r.size() > 1 && r.back() == '0') r.pop_back();
    reverse(all(r));
    return r;
}
string mulBig(const string& a, const string& b) {
    vi d(a.size() + b.size(), 0);
    for (int i = a.size() - 1; i >= 0; i--)
        for (int j = b.size() - 1; j >= 0; j--)
            d[i + j + 1] += (a[i] - '0') * (b[j] - '0');
    for (int k = d.size() - 1; k > 0; k--) {
        d[k - 1] += d[k] / 10;
        d[k] %= 10;
    }
    string r;
    for (int x : d) if (!r.empty() || x) r += char('0' + x);
    return r.empty() ? "0" : r;
}
// a / m and a % m for a small m (m up to 1e17)
string divSmall(const string& a, ll m, ll& rem) {
    string q;
    rem = 0;
    for (char c : a) {
        rem = rem * 10 + (c - '0');
        q += char('0' + rem / m);
        rem %= m;
    }
    size_t p = q.find_first_not_of('0');
    return p == string::npos ? "0" : q.substr(p);
}
