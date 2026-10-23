// lcm of all numbers in a (all >= 1), or -1 as soon as it passes
// LIM. Divide before you multiply: lcm(x, y) = x / gcd(x, y) * y
ll lcmAll(const vector<ll>& a, ll LIM = 2e18) {
    ll l = 1;
    for (ll x : a) {
        ll y = x / gcd(l, x);        // lcm(l, x) = l * y
        if (l > LIM / y) return -1;  // l * y > LIM
        l *= y;
    }
    return l;
}
