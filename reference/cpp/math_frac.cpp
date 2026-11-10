// exact fraction p / q, always in lowest terms with q > 0.
// Products like p * o.q must fit in ll. Print: f.p << "/" << f.q
struct Frac {
    ll p, q;
    Frac(ll a = 0, ll b = 1) {
        ll g = gcd(a, b);
        if (b < 0) g = -g;
        p = a / g, q = b / g;
    }
    Frac operator+(Frac o) { return {p * o.q + o.p * q, q * o.q}; }
    Frac operator-(Frac o) { return {p * o.q - o.p * q, q * o.q}; }
    Frac operator*(Frac o) { return {p * o.p, q * o.q}; }
    Frac operator/(Frac o) { return {p * o.q, q * o.p}; }
    bool operator<(Frac o) const { return p * o.q < o.p * q; }
    bool operator==(Frac o) const { return p == o.p && q == o.q; }
};
