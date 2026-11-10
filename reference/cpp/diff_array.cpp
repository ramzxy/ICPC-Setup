// many updates "add v to a[l..r]", then read every value once.
// O(1) per update, O(n) at the end
vector<ll> diff(n + 1, 0);
for (auto [l, r, v] : updates) { diff[l] += v; diff[r + 1] -= v; }
ll cur = 0;
rep(i, 0, n) { cur += diff[i]; a[i] += cur; }
