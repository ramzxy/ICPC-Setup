// matrix product and power mod MOD (needs MOD). For k x k matrices
// matpow takes O(k^3 log e)
using Mat = vector<vector<ll>>;
Mat matmul(const Mat& A, const Mat& B) {
    int n = A.size(), m = B[0].size(), k = B.size();
    Mat C(n, vector<ll>(m, 0));
    rep(i, 0, n) rep(j, 0, m) rep(l, 0, k)
        C[i][j] = (C[i][j] + A[i][l] * B[l][j]) % MOD;
    return C;
}
Mat matpow(Mat A, ll e) {
    int n = A.size();
    Mat R(n, vector<ll>(n, 0));
    rep(i, 0, n) R[i][i] = 1;    // identity
    while (e > 0) {
        if (e & 1) R = matmul(R, A);
        A = matmul(A, A);
        e >>= 1;
    }
    return R;
}
// Fibonacci: F(n) = matpow({{1, 1}, {1, 0}}, n)[0][1]
