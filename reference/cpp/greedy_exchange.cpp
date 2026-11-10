// one machine does the jobs {t, w} one after another; minimise the
// sum of w * (finish time). All t, w > 0. a goes before b if
// t_a / w_a < t_b / w_b; cross-multiplied: exact, no doubles
sort(all(jobs), [](auto& a, auto& b) {
    return a.first * b.second < b.first * a.second;
});
ll now = 0, cost = 0;
for (auto [t, w] : jobs) { now += t; cost += w * now; }
