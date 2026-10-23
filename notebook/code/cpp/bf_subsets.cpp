// try every subset of the n items (n <= 20): bit i of mask says
// if item i is in it. Example: can some of the a[i] sum to S?
bool found = false;
rep(mask, 0, 1 << n) {
    ll sum = 0;
    rep(i, 0, n) if (mask >> i & 1) sum += a[i];
    if (sum == S) found = true;
}
// number of items in the subset: __builtin_popcount(mask)
