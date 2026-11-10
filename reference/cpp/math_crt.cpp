// x = a1 (mod m1) and x = a2 (mod m2); m1, m2 need not be coprime.
// Returns {x, lcm(m1, m2)} with 0 <= x < lcm, or {-1, -1} if there
// is no such x. Needs egcd. The lcm must fit in ll
pair<ll, ll> crt(ll a1, ll m1, ll a2, ll m2) {
    ll p, q, g = egcd(m1, m2, p, q);  // m1 * p + m2 * q = g
    if ((a2 - a1) % g != 0) return {-1, -1};
    ll l = m1 / g * m2;
    // x = a1 + m1 * k, where m1 * k = a2 - a1 (mod m2)
    ll k = (__int128)((a2 - a1) / g) * p % (m2 / g);
    ll x = ((a1 + (__int128)m1 * k) % l + l) % l;
    return {x, l};
}
// more equations: merge one at a time, {x, m} = crt(x, m, a3, m3)
