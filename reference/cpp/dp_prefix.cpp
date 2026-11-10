// ways to climb n stairs taking 1..k stairs per step (mod 1e9+7).
// dp[i] = dp[i-k] + ... + dp[i-1] is a range sum, so keep
// pre[i] = dp[0] + ... + dp[i-1]: O(n) instead of O(n * k)
const ll MOD = 1e9 + 7;
vector<ll> dp(n + 1, 0), pre(n + 2, 0);
dp[0] = 1; pre[1] = 1;
rep(i, 1, n + 1) {
    dp[i] = (pre[i] - pre[max(0, i - k)] + MOD) % MOD;
    pre[i + 1] = (pre[i] + dp[i]) % MOD;
}
