ll mask = 0b10110;                         // bits 1, 2, 4 set
int ones = __builtin_popcountll(mask);     // 3
int lowest = __builtin_ctzll(mask);        // 1   (mask must be > 0)
int highest = 63 - __builtin_clzll(mask);  // 4   (mask must be > 0)
bool bit2 = mask >> 2 & 1;                 // is bit 2 set? true
mask |= 1LL << 5;                          // set bit 5
mask &= ~(1LL << 1);                       // clear bit 1
mask ^= 1;                                 // flip bit 0
int n = 3, subsets = 0;
rep(sub, 0, 1 << n) subsets++;             // all 8 subsets
int m = 0b101, parts = 0;                  // every non-empty submask
for (int sub = m; sub > 0; sub = (sub - 1) & m) parts++;  // 3
