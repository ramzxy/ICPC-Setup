// C[n][k] for n, k < M by Pascal's rule. Exact in ll up to n = 66;
// for any modulus (also not prime) add % m; use double for
// probabilities with n up to about 1000
int M = 67;
vector<vector<ll>> C(M, vector<ll>(M, 0));
rep(n, 0, M) {
    C[n][0] = 1;
    rep(k, 1, n + 1) C[n][k] = C[n - 1][k - 1] + C[n - 1][k];
}
