vi v = {5, 1, 4, 1};
sort(all(v));                      // 1 1 4 5
sort(all(v), greater<>());         // 5 4 1 1  (biggest first)
v.erase(unique(all(v)), v.end());  // drop repeats (sort first)
// pairs sort by .first, then .second. Own order with a lambda:
vector<pii> ps = {{1, 9}, {2, 3}, {3, 3}};
sort(all(ps), [](const pii& a, const pii& b) {
    if (a.second != b.second) return a.second < b.second;
    return a.first > b.first;      // tie: bigger .first first
});                                // {3,3} {2,3} {1,9}
struct Job { int start, end; };
vector<Job> jobs = {{1, 5}, {0, 5}, {2, 3}};
sort(all(jobs), [](const Job& a, const Job& b) {  // end, then start
    return tie(a.end, a.start) < tie(b.end, b.start);
});
// a comparator must be strict: use <, never <= (it can crash)
