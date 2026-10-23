// Checks the snippets of sections/strings.tex against brute force.
#include "check.hpp"
#include "../code/cpp/str_palindrome.cpp"
#include "../code/cpp/str_kmp.cpp"
#include "../code/cpp/str_hash.cpp"
#include "../code/cpp/str_trie.cpp"
#include "../code/cpp/str_parity.cpp"

array<int, 26> countOf(const string& s) {
#include "../code/cpp/str_count.cpp"
    array<int, 26> r;
    copy(cnt, cnt + 26, r.begin());
    return r;
}

// random string of length n over the first k letters
string rndStr(int n, int k) {
    string s;
    rep(i, 0, n) s += char('a' + rnd(0, k - 1));
    return s;
}
bool isPalBrute(const string& s) { return s == string(s.rbegin(), s.rend()); }

void testCountPalindrome() {
    rep(iter, 0, 3000) {
        string s = rndStr(rnd(0, 20), rnd(1, 26)), t = s;
        if (rnd(0, 1)) shuffle(all(t), rng);
        else t = rndStr(s.size(), 3);
        auto c = countOf(s);
        rep(i, 0, 26) CHECK(c[i] == count(all(s), char('a' + i)));
        string a = s, b = t;
        sort(all(a)), sort(all(b));
        CHECK((countOf(s) == countOf(t)) == (a == b));   // anagram rule
    }
    rep(iter, 0, 3000) {
        string s = rndStr(rnd(0, 16), rnd(1, 3));
        CHECK(isPal(s) == isPalBrute(s));
        int best = 0;   // brute force: try every substring
        rep(l, 0, (int)s.size()) rep(len, 1, (int)s.size() - l + 1)
            if (isPalBrute(s.substr(l, len))) best = max(best, len);
        auto [start, len] = longestPal(s);
        CHECK(len == best);
        CHECK(start >= 0 && start + len <= (int)s.size());
        CHECK(isPalBrute(s.substr(start, len)));
    }
    puts("  letter count, palindrome, longest palindromic substring OK");
}

void testKmp() {
    rep(iter, 0, 3000) {
        string s = rndStr(rnd(0, 18), rnd(1, 3));
        int n = s.size();
        vi pi = prefixFunction(s);
        CHECK((int)pi.size() == n);
        rep(i, 0, n) {   // longest k <= i with prefix == suffix of s[0..i]
            int want = 0;
            rep(k, 1, i + 1) if (s.substr(0, k) == s.substr(i + 1 - k, k)) want = k;
            CHECK(pi[i] == want);
        }
        string pat = rndStr(rnd(1, 4), rnd(1, 3));
        vi want;
        rep(i, 0, n - (int)pat.size() + 1) if (s.compare(i, pat.size(), pat) == 0) want.push_back(i);
        CHECK(findAll(pat, s) == want);
        if (n == 0) continue;
        int p = n - pi[n - 1], bp = 1;   // the period rule
        while (true) {
            bool ok = true;
            rep(i, 0, n - bp) if (s[i] != s[i + bp]) ok = false;
            if (ok) break;
            bp++;
        }
        CHECK(p == bp);
        bool repeated = false;   // s = some block of length d repeated n / d times
        rep(d, 1, n + 1) if (n % d == 0) {
            string t;
            rep(j, 0, n / d) t += s.substr(0, d);
            if (t == s && d == p) repeated = true;
            if (t == s) CHECK(d >= p);
        }
        CHECK(repeated == (n % p == 0));
    }
    puts("  prefix function, findAll, period OK");
}

ull mulSlow(ull a, ull b) {   // a * b mod HMOD by doubling: no overflow
    ull r = 0;
    a %= HMOD;
    while (b) {
        if (b & 1) r = (r + a) % HMOD;
        a = (a + a) % HMOD;
        b >>= 1;
    }
    return r;
}

void testHash() {
    rep(iter, 0, 2000) {
        ull a = rndll(0, HMOD - 1), b = rndll(0, HMOD - 1);
        if (iter < 4) a = HMOD - 1 - iter, b = HMOD - 1;
        CHECK(hmul(a, b) == mulSlow(a, b));
    }
    rep(iter, 0, 600) {
        string s = rndStr(rnd(0, 25), rnd(1, 2)), t = rndStr(rnd(0, 25), rnd(1, 2));
        Hash hs(s), ht(t);
        int n = s.size(), m = t.size();
        rep(l1, 0, n + 1) rep(r1, l1, n + 1) {
            rep(l2, 0, n + 1) rep(r2, l2, n + 1)
                CHECK((hs.get(l1, r1) == hs.get(l2, r2)) == (s.substr(l1, r1 - l1) == s.substr(l2, r2 - l2)));
            rep(l2, 0, m + 1) rep(r2, l2, m + 1)
                CHECK((hs.get(l1, r1) == ht.get(l2, r2)) == (s.substr(l1, r1 - l1) == t.substr(l2, r2 - l2)));
        }
    }
    string big = rndStr(200000, 2);   // long string: a few random comparisons
    Hash hb(big);
    rep(iter, 0, 20000) {
        int L = rnd(1, 30), i = rnd(0, 200000 - L), j = rnd(0, 200000 - L);
        if (iter % 2) j = i;
        CHECK((hb.get(i, i + L) == hb.get(j, j + L)) == (big.compare(i, L, big, j, L) == 0));
    }
    puts("  rolling hash OK");
}

void testTrie() {
    rep(iter, 0, 1000) {
        int k = rnd(1, 3);
        vector<string> words;
        Trie t;
        rep(i, 0, rnd(0, 20)) {
            words.push_back(rndStr(rnd(0, 5), k));
            t.insert(words.back());
        }
        rep(q, 0, 30) {
            string p = rndStr(rnd(0, 6), k + 1);   // may use a letter no word has
            int starts = 0, equal = 0;
            for (auto& w : words) {
                starts += w.compare(0, p.size(), p) == 0;
                equal += w == p;
            }
            int v = t.find(p);
            CHECK((v < 0 ? 0 : t.cnt[v]) == starts);
            CHECK((v < 0 ? 0 : t.ends[v]) == equal);
            if (starts > 0) CHECK(v >= 0);
        }
    }
    puts("  trie OK");
}

void testParity() {
    rep(iter, 0, 2000) {
        string s = rndStr(rnd(0, 7), rnd(1, 4)), t = s;
        sort(all(t));
        bool can = false;   // try every order of the letters
        do if (isPalBrute(t)) can = true; while (next_permutation(all(t)));
        CHECK(canBePal(s) == can);
    }
    rep(iter, 0, 2000) {
        string s = rndStr(rnd(0, 30), rnd(1, 26));
        if (iter % 3 == 0) s = rndStr(rnd(0, 30), 2);
        ll want = 0;
        rep(l, 0, (int)s.size()) {
            int c[26] = {};
            rep(r, l, (int)s.size()) {
                c[s[r] - 'a']++;
                int odd = 0;
                rep(x, 0, 26) odd += c[x] % 2;
                want += odd <= 1;
            }
        }
        CHECK(palSubstrings(s) == want);
    }
    CHECK(palSubstrings("zz") == 3);   // uses the top bit of the mask
    puts("  palindrome by letter parity OK");
}

int main() {
    testCountPalindrome();
    testKmp();
    testHash();
    testTrie();
    testParity();
}
