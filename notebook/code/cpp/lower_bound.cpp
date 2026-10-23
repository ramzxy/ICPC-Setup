// v sorted:
//   first index with v[i] >= x:  lower_bound(all(v), x) - v.begin()
//   first index with v[i] >  x:  upper_bound(all(v), x) - v.begin()
//   how often x occurs:  upper_bound(...) - lower_bound(...)
// set<ll> s:
//   first element >= x:  s.lower_bound(x)   (s.end() if none)
//   last element <= x:   auto it = s.upper_bound(x);
//                        if (it != s.begin()) { --it; use *it; }
