// which sums can a subset of w make? can[s] == 1 if some subset
// has sum s. O(n * S / 64): fast up to S = 10^6 and more
bitset<100001> can;  // size = largest sum + 1, must be a constant
can[0] = 1;
for (int x : w) can |= can << x;
