// maximum bipartite matching (Kuhn). Left nodes 0..nl-1, right
// nodes 0..nr-1, adj[l] = right nodes that l may be paired with.
// After solve(): mt[r] = partner of r (-1 = none). O(nl * m).
struct Matching {
    vector<vi> adj;
    vi mt;
    vector<bool> seen;
    Matching(const vector<vi>& a, int nr) : adj(a), mt(nr, -1) {}
    bool augment(int l) {   // find a partner for l, maybe by
        for (int r : adj[l]) if (!seen[r]) {   // moving others
            seen[r] = true;
            if (mt[r] == -1 || augment(mt[r])) {
                mt[r] = l;
                return true;
            }
        }
        return false;
    }
    int solve() {   // returns the number of pairs
        int pairs = 0;
        rep(l, 0, (int)adj.size()) {
            seen.assign(mt.size(), false);
            if (augment(l)) pairs++;
        }
        return pairs;
    }
};
