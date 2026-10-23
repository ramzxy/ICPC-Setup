// x >= 0 written in base b (2..36), digits 0-9 then a-z
string toBase(ll x, int b) {
    if (x == 0) return "0";
    string s;
    while (x > 0) {
        int d = x % b;
        s += d < 10 ? '0' + d : 'a' + d - 10;
        x /= b;
    }
    reverse(all(s));
    return s;
}
ll fromBase(const string& s, int b) {
    ll x = 0;
    for (char c : s)
        x = x * b + (isdigit(c) ? c - '0' : tolower(c) - 'a' + 10);
    return x;
}
// base 2 only: bitset<64>(x).to_string(), stoll(s, nullptr, 2)
