// the fraction p / q as one number mod MOD (prime), for "print
// P * Q^-1 mod MOD". Needs MOD and inv. frac(1, 2) == (MOD + 1) / 2
ll frac(ll p, ll q) { return (p % MOD + MOD) % MOD * inv(q) % MOD; }
