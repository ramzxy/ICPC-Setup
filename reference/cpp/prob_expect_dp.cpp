// expected-value DP. Example: you stand on square i of 0..n; each
// turn throw a fair die 1..k and move forward, but a throw past n
// is wasted (you stay). E[i] = expected turns from i until n
vector<double> E(n + 1, 0);  // E[n] = 0: already there
for (int i = n - 1; i >= 0; i--) {
    double sum = 0, stay = 0;  // stay = P(this throw is wasted)
    rep(r, 1, k + 1) {
        if (i + r <= n) sum += E[i + r] / k;
        else stay += 1.0 / k;
    }
    // E[i] = 1 + sum + stay * E[i]: move stay * E[i] to the left
    E[i] = (1 + sum) / (1 - stay);
}
