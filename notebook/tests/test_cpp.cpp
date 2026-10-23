// Checks the snippets of sections/cpp.tex: each one is pasted into a
// function, then its variables are compared with the values its
// comments promise.
#include "check.hpp"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#include "../code/cpp/stl_oset.cpp"
#include "../code/cpp/stl_pairhash.cpp"

void testOset() {   // against a multiset, with duplicates stored as {value, id}
    rep(iter, 0, 200) {
        oset<pii> s;
        multiset<int> ms;
        int id = 0;
        rep(op, 0, 200) {
            if (rnd(0, 2) == 0 && !ms.empty()) {
                auto it = ms.begin();
                advance(it, rnd(0, (int)ms.size() - 1));
                int v = *it;
                ms.erase(it);
                s.erase(s.lower_bound({v, INT_MIN}));
            } else {
                int x = rnd(0, 20);
                ms.insert(x);
                s.insert({x, id++});
            }
            CHECK(s.size() == ms.size());
            int q = rnd(-1, 21);
            CHECK((int)s.order_of_key({q, INT_MIN}) == (int)distance(ms.begin(), ms.lower_bound(q)));
            if (!ms.empty()) {
                int k = rnd(0, (int)ms.size() - 1);
                auto it = ms.begin();
                advance(it, k);
                CHECK(s.find_by_order(k)->first == *it);
            }
        }
    }
}
void testPairHash() {
    unordered_map<pair<ll, ll>, int, PairHash> seen;
    map<pair<ll, ll>, int> ref;
    rep(i, 0, 20000) {
        pair<ll, ll> k = {rndll(-50, 50), rndll(-50, 50)};
        seen[k]++;
        ref[k]++;
    }
    CHECK(seen.size() == ref.size());
    for (auto& [k, v] : ref) CHECK(seen[k] == v);
}

void testSort() {
#include "../code/cpp/stl_sort.cpp"
    CHECK((v == vi{5, 4, 1}));
    CHECK((ps == vector<pii>{{3, 3}, {2, 3}, {1, 9}}));
    CHECK(jobs[0].end == 3 && jobs[1].start == 0 && jobs[2].start == 1);
}
void testAlgo() {
#include "../code/cpp/stl_algo.cpp"
    CHECK(mx == 9 && pos == 1 && total == 23 && nines == 2 && has2);
    CHECK(low == 1 && orders == 24 && g == 6 && l == 12);
}
void testContainers() {
#include "../code/cpp/stl_containers.cpp"
    CHECK(has3 && *it == 5 && first == 1 && last == 5);
    CHECK(ms.size() == 2 && ms.count(2) == 1);
    CHECK(!pear && cnt.size() == 1 && cnt["apple"] == 1);
    CHECK(big == 9 && small == 4);
    CHECK((dq == deque<int>{0, 1}));
    CHECK(grid.size() == 3 && grid[0].size() == 4 && grid[2][3] == 0);
}
void testStrings() {
#include "../code/cpp/stl_strings.cpp"
    CHECK(w == "World" && at == 3 && !found);
    CHECK(x == 42 && y == 12345678901LL && d == 3.5 && t == "17");
    CHECK(k == 4 && back == 'e' && dig && let);
    CHECK(s == "hello world" && stars == "***" && r == "dlrow olleh");
}
void testBits() {
#include "../code/cpp/stl_bits.cpp"
    CHECK(ones == 3 && lowest == 1 && highest == 4 && bit2);
    CHECK(mask == 0b110101);
    CHECK(subsets == 8 && parts == 3);
}
void testLambda() {
#include "../code/cpp/stl_lambda.cpp"
    CHECK(sq(7) == 49 && calls == 2 && f10 == 55);
}

int main() {
    testSort();
    testAlgo();
    testContainers();
    testStrings();
    testBits();
    testLambda();
    puts("  sorting, algorithms, containers, strings, bits, lambdas OK");
    testOset();
    testPairHash();
    puts("  ordered set, pair hash OK");
}
