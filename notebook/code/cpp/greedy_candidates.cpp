// customer i buys if the price is <= b[i]. One price p for all:
// earn p * (number of b[i] >= p). The best p is one of the b[i]
// (raise p up to the next b[i]: same buyers, more money)
sort(all(b), greater<>());  // biggest first
ll best = 0;
rep(i, 0, n) best = max(best, b[i] * (i + 1));  // i+1 buyers
