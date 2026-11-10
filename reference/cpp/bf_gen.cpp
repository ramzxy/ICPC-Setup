// random test generator. "./gen 5 > in.txt" writes test 5: the
// same seed always gives the same test. Put the template above.
int main(int argc, char* argv[]) {
    mt19937 rng(atoi(argv[1]));
    auto rnd = [&](int lo, int hi) {   // random int in [lo, hi]
        return uniform_int_distribution<int>(lo, hi)(rng);
    };
    int n = rnd(1, 8);   // keep tests SMALL: easy to check by hand
    cout << n << '\n';
    rep(i, 0, n) cout << rnd(1, 10) << " \n"[i == n - 1];
    // tree on nodes 1..n, one line "parent child" per edge:
    // rep(v, 2, n + 1) cout << rnd(1, v - 1) << ' ' << v << '\n';
    // string: rep(i, 0, n) cout << char('a' + rnd(0, 2));
    // permutation: vi p(n); iota(all(p), 1); shuffle(all(p), rng);
}
