// pre[i] = a[0] + ... + a[i-1]
// sum of a[l..r] (inclusive) = pre[r + 1] - pre[l]
vector<ll> pre(n + 1, 0);
rep(i, 0, n) pre[i + 1] = pre[i] + a[i];
