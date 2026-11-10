// jobs {d, p}: each takes 1 time unit and earns p >= 0 if it is
// done by time d (slots 1, 2, ..). Most profit: go by deadline,
// keep the jobs taken in a min-heap, drop the cheapest if too many
ll maxProfit(vector<pair<ll, ll>> jobs) {
    sort(all(jobs));
    priority_queue<ll, vector<ll>, greater<>> pq;  // min on top
    ll sum = 0;
    for (auto [d, p] : jobs) {
        pq.push(p); sum += p;
        if ((ll)pq.size() > d) { sum -= pq.top(); pq.pop(); }
    }
    return sum;
}
// jobs {d, len}: one at a time from time 0, must end by time d.
// Most jobs done: go by deadline; if we are late, drop the
// longest job taken so far
int maxJobs(vector<pair<ll, ll>> jobs) {
    sort(all(jobs));
    priority_queue<ll> pq;  // largest on top
    ll now = 0;
    for (auto [d, len] : jobs) {
        pq.push(len); now += len;
        if (now > d) { now -= pq.top(); pq.pop(); }
    }
    return pq.size();
}
