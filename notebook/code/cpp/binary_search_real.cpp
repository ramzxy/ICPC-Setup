// real-valued answer: halve the range 100 times (plenty of digits)
double lo = 0, hi = 1e9;
rep(it, 0, 100) {
    double mid = (lo + hi) / 2;
    if (ok(mid)) hi = mid;
    else lo = mid;
}
// answer = hi
