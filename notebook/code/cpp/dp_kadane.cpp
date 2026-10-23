// largest sum of a block a[l..r] of consecutive elements, not
// empty (if the empty block counts, start with best = 0).
// cur = largest sum of a block that ends at the current x
ll best = -INF, cur = 0;
for (ll x : a) {
    cur = max(x, cur + x);  // extend the block, or start anew at x
    best = max(best, cur);
}
