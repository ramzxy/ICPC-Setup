// longest subarray with sum <= K (all a[i] >= 0). The window is
// a[l..r]: r moves right, l catches up while the sum is too big.
// O(n): every index enters and leaves the window once
int best = 0;
ll sum = 0;
for (int l = 0, r = 0; r < n; r++) {
    sum += a[r];
    while (sum > K) sum -= a[l++];
    best = max(best, r - l + 1);
}
