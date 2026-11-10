// coins (different values) may be used any number of times.
// fewest[s] = fewest coins with sum exactly s (INF: impossible)
// ways[s]  = ways to pay s, 1+2 and 2+1 count once (mod 1e9+7)
// order[s] = ways to pay s, 1+2 and 2+1 count twice. O(n * S)
const ll MOD = 1e9 + 7;
vector<ll> fewest(S + 1, INF), ways(S + 1, 0), order(S + 1, 0);
fewest[0] = 0; ways[0] = order[0] = 1;
for (int c : coins)  // coins outside
    for (int s = c; s <= S; s++) {  // forwards: c can be reused
        fewest[s] = min(fewest[s], fewest[s - c] + 1);
        ways[s] = (ways[s] + ways[s - c]) % MOD;
    }
rep(s, 1, S + 1) for (int c : coins)  // sums outside
    if (c <= s) order[s] = (order[s] + order[s - c]) % MOD;
