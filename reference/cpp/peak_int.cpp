// integers in [lo, hi], f goes up then down (no flat parts!):
// the peak is the first x whose next value is lower. Needs firstTrue
ll peak = firstTrue(lo, hi - 1,
                    [&](ll x) { return f(x) > f(x + 1); });
