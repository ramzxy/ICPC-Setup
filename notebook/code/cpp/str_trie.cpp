// trie of lowercase words. Node 0 = root = the empty prefix.
// cnt[v] = number of words that start with the prefix of node v,
// ends[v] = number of words equal to it
struct Trie {
    vector<array<int, 26>> nxt;   // nxt[v][c] = child, 0 = none
    vi cnt, ends;
    Trie() { newNode(); }
    int newNode() {
        nxt.push_back({}), cnt.push_back(0), ends.push_back(0);
        return nxt.size() - 1;
    }
    void insert(const string& w) {
        int v = 0;
        cnt[0]++;
        for (char ch : w) {
            int c = ch - 'a';
            if (nxt[v][c] == 0) {
                int u = newNode();   // first: it resizes nxt
                nxt[v][c] = u;
            }
            v = nxt[v][c];
            cnt[v]++;
        }
        ends[v]++;
    }
    int find(const string& p) {   // node of prefix p, -1 if none
        int v = 0;
        for (char ch : p) {
            v = nxt[v][ch - 'a'];
            if (v == 0) return -1;
        }
        return v;
    }
};
// words starting with p: int v = t.find(p); v < 0 ? 0 : t.cnt[v]
