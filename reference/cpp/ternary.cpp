// real numbers, f goes up then down: returns x of the maximum
// (for a minimum, use -f)
template <class F> double ternaryMax(double lo, double hi, F f) {
    rep(it, 0, 200) {
        double m1 = lo + (hi - lo) / 3, m2 = hi - (hi - lo) / 3;
        if (f(m1) < f(m2)) lo = m1;  // peak is right of m1
        else hi = m2;                 // peak is left of m2
    }
    return lo;
}
