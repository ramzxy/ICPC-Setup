// smallest x in [lo, hi] with ok(x) true; ok looks like F..F T..T.
// returns hi + 1 if ok is false everywhere
template <class F> ll firstTrue(ll lo, ll hi, F ok) {
    hi++;
    while (lo < hi) {
        ll mid = lo + (hi - lo) / 2;
        if (ok(mid)) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}
// use: ll ans = firstTrue(0, 2e9, [&](ll t) { return canDo(t); });
// largest x with ok(x), where ok looks like T..T F..F:
//        firstTrue(lo, hi, [&](ll x) { return !ok(x); }) - 1
