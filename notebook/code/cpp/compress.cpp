// replace values by their rank 0..k-1 among the distinct values,
// keeping their order (big values -> small array indices)
vector<ll> vals(all(a));
sort(all(vals));
vals.erase(unique(all(vals)), vals.end());   // k = vals.size()
vi id(n);
rep(i, 0, n) id[i] = lower_bound(all(vals), a[i]) - vals.begin();
// original value back: vals[id[i]]
