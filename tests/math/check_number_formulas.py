"""Bounded, independent numerical checks for the supplement's maths.

Expected results use integer enumeration, exact Fraction probabilities,
math.comb/factorial, or direct arithmetic rather than the formula under test.
"""
from collections import Counter, defaultdict
from fractions import Fraction as Q
from itertools import combinations, product
from math import comb, factorial, gcd, isqrt, lcm, prod

checks = Counter()


def check(family, condition, detail=None):
    checks[family] += 1
    if not condition:
        raise AssertionError((family, detail))


def factors(n):
    out = []
    p = 2
    while p * p <= n:
        e = 0
        while n % p == 0:
            n //= p
            e += 1
        if e:
            out.append((p, e))
        p += 1
    if n > 1:
        out.append((n, 1))
    return out


def divisors(n):
    return [d for d in range(1, n + 1) if n % d == 0]


def phi(n):
    return sum(gcd(a, n) == 1 for a in range(1, n + 1))


def mu(n):
    f = factors(n)
    return 0 if any(e > 1 for _, e in f) else (-1) ** len(f)


def vp(n, p):
    assert n > 0
    e = 0
    while n % p == 0:
        e += 1
        n //= p
    return e


def choose(n, k):
    return comb(n, k) if 0 <= k <= n else 0


for n in range(1, 121):
    fs = factors(n)
    check("totients", phi(n) == prod(p ** (e - 1) * (p - 1) for p, e in fs), n)
    check("totients", sum(phi(d) for d in divisors(n)) == n, n)
    check("mobius", sum(mu(d) for d in divisors(n)) == (n == 1), n)
    for f in (lambda t: t * t - 3 * t, lambda t: (7 * t * t + 3 * t) % 11 - 5):
        F = lambda t: sum(f(d) for d in divisors(t))
        check("mobius", f(n) == sum(mu(d) * F(n // d) for d in divisors(n)), n)

for a in range(1, 31):
    for b in range(1, 31):
        if gcd(a, b) == 1:
            check("totients", phi(a * b) == phi(a) * phi(b), (a, b))

for m in range(2, 61):
    ph = phi(m)
    for a in range(-10, m + 10):
        if gcd(a, m) == 1:
            check("euler-inverses", pow(a, ph, m) == 1, (a, m))
            check("euler-inverses", a * pow(a, ph - 1, m) % m == 1, (a, m))
            for e in range(15):
                check("euler-inverses", pow(a, e, m) == pow(a, e % ph, m), (a, e, m))
            brute = next(d for d in range(1, ph + 1) if pow(a, d, m) == 1)
            d = ph
            for q, _ in factors(ph):
                while d % q == 0 and pow(a, d // q, m) == 1:
                    d //= q
            check("multiplicative-order", d == brute and ph % d == 0, (a, m))

for m in range(2, 46):
    for a in range(1, 31):
        for e in range(13):
            residues = []
            for p, k in factors(m):
                modulus, u, s = p ** k, a, 0
                while u % p == 0:
                    u //= p
                    s += 1
                residue = 0 if s * e >= k else (p ** (s * e) * pow(u, e % phi(modulus), modulus)) % modulus
                check("noncoprime-powers", residue == pow(a, e, modulus), (a, e, modulus))
                residues.append((modulus, residue))
            reconstructed = [x for x in range(m) if all(x % mod == r for mod, r in residues)]
            check("noncoprime-powers", reconstructed == [pow(a, e, m)], (a, e, m))

for m in range(1, 36):
    for a in range(-10, 11):
        g = gcd(a, m)
        for b in range(-10, 11):
            actual = {x for x in range(m) if (a * x - b) % m == 0}
            check("congruences", len(actual) == (g if b % g == 0 else 0), (a, b, m))
            if actual:
                x0 = min(actual)
                check("congruences", actual == {(x0 + t * (m // g)) % m for t in range(g)}, (a, b, m))
        for x, y in product(range(-3, 4), repeat=2):
            check("congruences", ((a * (x - y)) % m == 0) == ((x - y) % (m // g) == 0), (a, x, y, m))

for m in range(1, 13):
    for n in range(1, 13):
        for a in range(m):
            for b in range(n):
                xs = [x for x in range(lcm(m, n)) if x % m == a and x % n == b]
                check("crt", len(xs) == (1 if (b - a) % gcd(m, n) == 0 else 0), (a, m, b, n))

for N in range(1, 26):
    for M in range(1, 26):
        pairs = [gcd(x, y) for x in range(1, N + 1) for y in range(1, M + 1)]
        h = lambda d: (N // d) * (M // d)
        check("gcd-pairs", sum(g == 1 for g in pairs) == sum(mu(d) * h(d) for d in range(1, min(N, M) + 1)), (N, M))
        check("gcd-pairs", sum(pairs) == sum(phi(d) * h(d) for d in range(1, min(N, M) + 1)), (N, M))
        for g in range(1, 7):
            actual = sum(v == g for v in pairs)
            expected = sum(mu(d) * (N // g // d) * (M // g // d) for d in range(1, min(N // g, M // g) + 1))
            check("gcd-pairs", actual == expected, (N, M, g))
        check("gcd-pairs", sum(gcd(x, M) == 1 for x in range(1, N + 1)) == sum(mu(d) * (N // d) for d in divisors(M)), (N, M))

tau = [0] + [len(divisors(n)) for n in range(1, 501)]
for N in range(1, 501):
    l, blocks, expanded = 1, 0, []
    while l <= N:
        q = N // l
        r = N // q
        expanded.extend([q] * (r - l + 1))
        l = r + 1
        blocks += 1
    actual = [N // i for i in range(1, N + 1)]
    s = isqrt(N)
    check("floor-blocks", expanded == actual and blocks <= 2 * s, N)
    check("floor-blocks", sum(actual) == sum(tau[1:N + 1]) == 2 * sum(N // i for i in range(1, s + 1)) - s * s, N)

for p in (2, 3, 5, 7, 11):
    for n in range(101):
        power, legendre = p, 0
        while power <= n:
            legendre += n // power
            power *= p
        check("valuations", legendre == vp(factorial(n), p), (p, n))
        for r in range(n + 1):
            check("valuations", vp(comb(n, r), p) == vp(factorial(n), p) - vp(factorial(r), p) - vp(factorial(n - r), p), (p, n, r))
            nn, rr, lucas = n, r, 1
            while nn or rr:
                lucas = lucas * choose(nn % p, rr % p) % p
                nn //= p
                rr //= p
            check("lucas", lucas == comb(n, r) % p, (p, n, r))
            if n <= 35:
                e = vp(factorial(n), p) - vp(factorial(r), p) - vp(factorial(n - r), p)
                U = lambda t: factorial(t) // p ** vp(factorial(t), p)
                for k in range(1, 5):
                    mod = p ** k
                    value = 0 if e >= k else (p ** e * U(n) * pow(U(r), -1, mod) * pow(U(n - r), -1, mod)) % mod
                    check("prime-power-binomials", value == comb(n, r) % mod, (p, k, n, r))

for n in range(50):
    for base in range(2, 26):
        f, zeros = factorial(n), 0
        while f % base == 0:
            zeros += 1
            f //= base
        formula = min(vp(factorial(n), p) // e for p, e in factors(base))
        check("valuations", zeros == formula, (n, base))

for d, m, quotient in product(range(1, 16), range(1, 26), range(-35, 36)):
    A = d * quotient
    check("exact-division", quotient % m == (A % (d * m)) // d, (A, d, m))

for n in range(31):
    for r in [Q(i) for i in range(-5, 6) if i != 1] + [Q(1, 2), Q(-2, 3)]:
        direct = sum(i * r ** i for i in range(1, n + 1))
        formula = r * (1 - (n + 1) * r ** n + n * r ** (n + 1)) / (1 - r) ** 2
        check("weighted-sums", direct == formula, (r, n))

for n in range(6):
    for a in product(range(-2, 3), repeat=n):
        pairs = list(combinations(a, 2))
        s = sorted(a)
        check("pair-sums", sum(abs(x - y) for x, y in pairs) == sum((2 * i - n - 1) * x for i, x in enumerate(s, 1)), a)
        check("pair-sums", sum((x - y) ** 2 for x, y in pairs) == n * sum(x * x for x in a) - sum(a) ** 2, a)
        check("pair-sums", 2 * sum(x * y for x, y in pairs) == sum(a) ** 2 - sum(x * x for x in a), a)

fib = [0, 1]
for _ in range(300):
    fib.append(fib[-1] + fib[-2])
for n in range(151):
    check("fibonacci", fib[2 * n] == fib[n] * (2 * fib[n + 1] - fib[n]), n)
    check("fibonacci", fib[2 * n + 1] == fib[n] ** 2 + fib[n + 1] ** 2, n)
    check("fibonacci", sum(fib[:n + 1]) == fib[n + 2] - 1, n)
    check("fibonacci", sum(x * x for x in fib[:n + 1]) == fib[n] * fib[n + 1], n)
for a, b in product(range(1, 101), repeat=2):
    check("fibonacci", gcd(fib[a], fib[b]) == fib[gcd(a, b)], (a, b))

for n in range(41):
    for k in range(n + 4):
        check("binomial-identities", sum(choose(i, k) for i in range(n + 1)) == choose(n + 1, k + 1), (n, k))
    check("binomial-identities", sum(comb(n, k) ** 2 for k in range(n + 1)) == comb(2 * n, n), n)
    if n:
        check("binomial-identities", sum(k * comb(n, k) for k in range(n + 1)) == n * 2 ** (n - 1), n)
        check("binomial-identities", sum((-1) ** k * comb(n, k) for k in range(n + 1)) == 0, n)
for a, b in product(range(13), repeat=2):
    for r in range(a + b + 3):
        check("binomial-identities", sum(choose(a, k) * choose(b, r - k) for k in range(a + 1)) == choose(a + b, r), (a, b, r))

for c, d, a0, a1 in product(range(-2, 3), repeat=4):
    seq = [a0, a1]
    for _ in range(12):
        seq.append(c * seq[-1] + d * seq[-2])
    for n in range(len(seq)):
        coefficient = seq[n] - (c * seq[n - 1] if n >= 1 else 0) - (d * seq[n - 2] if n >= 2 else 0)
        expected = a0 if n == 0 else a1 - c * a0 if n == 1 else 0
        check("generating-functions", coefficient == expected, (c, d, a0, a1, n))
for k in range(1, 5):
    for n in range(8):
        count = sum(sum(xs) == n for xs in product(range(n + 1), repeat=k))
        check("generating-functions", count == comb(n + k - 1, k - 1), (n, k))


def moments(pmf):
    assert sum(pmf.values()) == 1
    mean = sum(x * p for x, p in pmf.items())
    second = sum(x * x * p for x, p in pmf.items())
    variance = sum((x - mean) ** 2 * p for x, p in pmf.items())
    check("probability-moments", variance == second - mean * mean)
    check("probability-tails", mean == sum(sum(p for x, p in pmf.items() if x >= k) for k in range(1, max(pmf) + 1)))
    check("probability-tails", second == sum((2 * k - 1) * sum(p for x, p in pmf.items() if x >= k) for k in range(1, max(pmf) + 1)))
    for t in (Q(1, 2), Q(1), Q(3, 2), Q(2), Q(3)):
        check("probability-bounds", sum(p for x, p in pmf.items() if x >= t) <= mean / t)
        check("probability-bounds", sum(p for x, p in pmf.items() if abs(x - mean) >= t) <= variance / (t * t))
    return mean, variance


for n in range(8):
    for p in (Q(1, 4), Q(1, 3), Q(1, 2), Q(2, 3), Q(1)):
        pmf = defaultdict(Q)
        for bits in product((0, 1), repeat=n):
            pmf[sum(bits)] += prod(p if bit else 1 - p for bit in bits)
        mean, variance = moments(pmf)
        check("binomial-distribution", mean == n * p and variance == n * p * (1 - p), (n, p))
        for k in range(n + 1):
            check("binomial-distribution", pmf[k] == comb(n, k) * p ** k * (1 - p) ** (n - k), (n, p, k))

for N in range(2, 11):
    for K in range(N + 1):
        for n in range(N + 1):
            counts = Counter(sum(x < K for x in chosen) for chosen in combinations(range(N), n))
            pmf = {k: Q(count, comb(N, n)) for k, count in counts.items()}
            mean, variance = moments(pmf)
            p = Q(K, N)
            check("hypergeometric", mean == n * p and variance == n * p * (1 - p) * Q(N - n, N - 1), (N, K, n))
            for k in range(n + 1):
                check("hypergeometric", pmf.get(k, Q()) == Q(choose(K, k) * choose(N - K, n - k), comb(N, n)), (N, K, n, k))

for p in (Q(1, 3), Q(1, 2), Q(2, 3), Q(1)):
    partial_mean = sum(k * p * (1 - p) ** (k - 1) for k in range(1, 121))
    partial_second = sum(k * k * p * (1 - p) ** (k - 1) for k in range(1, 121))
    check("geometric-truncation", abs(partial_mean - 1 / p) < Q(1, 10 ** 15), p)
    check("geometric-truncation", abs(partial_second - (2 - p) / (p * p)) < Q(1, 10 ** 12), p)

states = [(x, y) for x, y in product(range(-2, 3), repeat=2)]
weights = [1 + ((x + 3) * (y + 3)) % 5 for x, y in states]
ps = [Q(w, sum(weights)) for w in weights]
EX = sum(x * p for (x, y), p in zip(states, ps))
EY = sum(y * p for (x, y), p in zip(states, ps))
VX = sum((x - EX) ** 2 * p for (x, y), p in zip(states, ps))
VY = sum((y - EY) ** 2 * p for (x, y), p in zip(states, ps))
COV = sum(x * y * p for (x, y), p in zip(states, ps)) - EX * EY
for a, b in product(range(-2, 3), repeat=2):
    actual = sum((a * x + b * y - a * EX - b * EY) ** 2 * p for (x, y), p in zip(states, ps))
    check("covariance", actual == a * a * VX + b * b * VY + 2 * a * b * COV, (a, b))

px, py = {0: Q(1, 3), 2: Q(2, 3)}, {1: Q(1, 4), 3: Q(3, 4)}
for t in range(-1, 5):
    maxima = sum(p * q for x, p in px.items() for y, q in py.items() if max(x, y) <= t)
    minima = sum(p * q for x, p in px.items() for y, q in py.items() if min(x, y) > t)
    check("independent-extremes", maxima == sum(p for x, p in px.items() if x <= t) * sum(p for y, p in py.items() if y <= t), t)
    check("independent-extremes", minima == sum(p for x, p in px.items() if x > t) * sum(p for y, p in py.items() if y > t), t)

for family, count in checks.items():
    print(f"{family}: {count:,} checks OK")
print(f"TOTAL: {sum(checks.values()):,} checks across {len(checks)} families OK")
