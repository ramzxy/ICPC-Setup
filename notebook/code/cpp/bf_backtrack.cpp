// backtracking with pruning. Example: put every job on one of the
// machines so that the busiest machine has as little work as
// possible. Sort the jobs big to small first: prunes much more.
vector<ll> job, load;   // load[k] = total work on machine k
ll best = INF;          // best answer found so far
void place(int i, ll mx) {   // jobs 0..i-1 placed, mx = max load
    if (mx >= best) return;  // prune: cannot beat the best
    if (i == (int)job.size()) { best = mx; return; }
    rep(k, 0, (int)load.size()) {
        load[k] += job[i];                // choose
        place(i + 1, max(mx, load[k]));   // go deeper
        load[k] -= job[i];                // undo
        if (load[k] == 0) break;   // empty machines are all alike
    }
}
// sort(job.rbegin(), job.rend()); load.assign(machines, 0);
// best = INF; place(0, 0);  // answer in best
