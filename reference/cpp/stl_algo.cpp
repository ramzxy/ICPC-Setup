vi w = {3, 9, 2, 9};
int mx = *max_element(all(w));              // 9
int pos = max_element(all(w)) - w.begin();  // 1 (the first max)
ll total = accumulate(all(w), 0LL);         // 0LL: sum as long long
int nines = count(all(w), 9);               // 2
bool has2 = find(all(w), 2) != w.end();     // true
int low = min({4, 7, 1});                   // min of many: braces
vi idx(4);
iota(all(idx), 0);                          // 0 1 2 3
int orders = 0;                             // every order, if sorted
do orders++; while (next_permutation(all(idx)));  // 24
ll g = gcd(12LL, 18LL), l = lcm(4LL, 6LL);  // 6 and 12
