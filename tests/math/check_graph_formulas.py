"""Independent small-case checks for graphs-and-counting.tex; no dependencies."""
from collections import Counter
from fractions import Fraction
from functools import lru_cache
from itertools import combinations, permutations, product
from math import comb, factorial, gcd, prod

stats = Counter()


def equal(actual, expected, context):
    assert actual == expected, (context, actual, expected)
    stats[context.split(':')[0]] += 1


def pairs(n):
    return list(combinations(range(n), 2))


def components(n, edges):
    adj = [[] for _ in range(n)]
    for u, v in edges:
        adj[u].append(v)
        adj[v].append(u)
    unseen = set(range(n))
    answer = 0
    while unseen:
        answer += 1
        stack = [unseen.pop()]
        while stack:
            for v in adj[stack.pop()]:
                if v in unseen:
                    unseen.remove(v)
                    stack.append(v)
    return answer


@lru_cache(None)
def cycles(n):
    """Enumerate all undirected simple cycles as canonical traversals."""
    edge_index = {e: i for i, e in enumerate(pairs(n))}
    result = []
    for k in range(3, n + 1):
        for vertices in combinations(range(n), k):
            for tail in permutations(vertices[1:]):
                if tail[0] > tail[-1]:
                    continue
                order = (vertices[0],) + tail
                mask = 0
                for i in range(k):
                    edge = tuple(sorted((order[i], order[(i + 1) % k])))
                    mask |= 1 << edge_index[edge]
                result.append((k, mask))
    return result


def determinant(matrix):
    """Leibniz definition, deliberately independent of Gaussian elimination."""
    n = len(matrix)
    total = 0
    for order in permutations(range(n)):
        inversions = sum(order[i] > order[j] for i in range(n) for j in range(i + 1, n))
        total += (-1 if inversions % 2 else 1) * prod(matrix[i][order[i]] for i in range(n))
    return total


def matmul(a, b):
    n = len(a)
    return [[sum(a[i][k] * b[k][j] for k in range(n)) for j in range(n)] for i in range(n)]


def trace(a):
    return sum(a[i][i] for i in range(len(a)))


def graph_checks():
    for n in range(1, 6):
        possible = pairs(n)
        tree_masks = []
        for chosen in combinations(range(len(possible)), n - 1):
            if components(n, [possible[i] for i in chosen]) == 1:
                tree_masks.append(sum(1 << i for i in chosen))
        for graph in range(1 << len(possible)):
            edges = [edge for i, edge in enumerate(possible) if graph >> i & 1]
            m = len(edges)
            c = components(n, edges)
            a = [[0] * n for _ in range(n)]
            for u, v in edges:
                a[u][v] = a[v][u] = 1
            degrees = [sum(row) for row in a]
            equal(sum(degrees), 2 * m, 'handshake')
            equal(sum(d % 2 for d in degrees) % 2, 0, 'odd-degree-parity')
            actual_cycles = [(k, mask) for k, mask in cycles(n) if mask & graph == mask]
            counts = Counter(k for k, _ in actual_cycles)
            common = [[sum(a[u][v] * a[w][v] for v in range(n)) for w in range(n)] for u in range(n)]
            equal(sum(common[u][v] for u, v in edges), 3 * counts[3], 'triangles-common')
            equal(sum(comb(common[u][v], 2) for u, v in possible), 2 * counts[4], 'fourcycles-common')
            a2 = matmul(a, a)
            a3 = matmul(a2, a)
            a4 = matmul(a2, a2)
            equal(trace(a3), 6 * counts[3], 'triangles-trace')
            equal(trace(a4) + 2 * m - 2 * sum(d * d for d in degrees), 8 * counts[4], 'fourcycles-trace')
            parity = [0] * (1 << m)
            edge_parity = [(1 << u) | (1 << v) for u, v in edges]
            for subset in range(1, 1 << m):
                bit = subset & -subset
                parity[subset] = parity[subset ^ bit] ^ edge_parity[bit.bit_length() - 1]
            equal(parity.count(0), 1 << (m - n + c), 'cycle-space')
            cuts = set()
            for side in range(1 << n):
                cut = sum(1 << i for i, (u, v) in enumerate(possible)
                          if graph >> i & 1 and ((side >> u) ^ (side >> v)) & 1)
                cuts.add(cut)
                for _, mask in actual_cycles:
                    equal((cut & mask).bit_count() % 2, 0, 'cut-cycle-parity')
            equal(len(cuts), 1 << (n - c), 'cut-space')
            for i, edge in enumerate(possible):
                if not graph >> i & 1:
                    continue
                is_bridge = components(n, [e for e in edges if e != edge]) > c
                no_cycle = all(not mask >> i & 1 for _, mask in actual_cycles)
                equal(is_bridge, no_cycle, 'bridges')
            laplacian = [[degrees[i] if i == j else -a[i][j] for j in range(n)] for i in range(n)]
            expected = sum(mask & graph == mask for mask in tree_masks)
            for root in range(n):
                cofactor = [[laplacian[i][j] for j in range(n) if j != root] for i in range(n) if i != root]
                equal(determinant(cofactor), expected, 'matrix-tree-cofactors')
            equal(determinant(laplacian), 0, 'full-laplacian-determinant')
            stats['graphs-enumerated'] += 1


def complete_checks():
    for n in range(3, 9):
        counts = Counter(k for k, _ in cycles(n))
        for k in range(3, n + 1):
            equal(counts[k], factorial(n) // (2 * k * factorial(n - k)), 'complete-cycles')
        equal(counts[n], factorial(n - 1) // 2, 'hamilton-cycles')
    for n in range(2, 7):
        for k in range(2, n + 1):
            # Every selected labeled set admits all cycles fixing the least start.
            directed = sum(1 for chosen in combinations(range(n), k) for _ in permutations(chosen[1:]))
            equal(directed, factorial(n) // (k * factorial(n - k)), 'directed-complete-cycles')
    for left in range(1, 5):
        for right in range(1, 5):
            n = left + right
            possible = pairs(n)
            graph = sum(1 << i for i, (u, v) in enumerate(possible) if u < left <= v)
            counts = Counter(k for k, mask in cycles(n) if mask & graph == mask)
            for k in range(2, min(left, right) + 1):
                expected = comb(left, k) * comb(right, k) * factorial(k) * factorial(k - 1) // 2
                equal(counts[2 * k], expected, 'complete-bipartite-cycles')
            a = [[int((u < left) != (v < left)) for v in range(n)] for u in range(n)]
            common_side = sum(comb(sum(a[u][v] * a[w][v] for v in range(n)), 2)
                              for u, w in combinations(range(left), 2))
            equal(common_side, counts[4], 'bipartite-one-side-fourcycles')
            # Tree enumeration for modest sizes; independently choose edge sets.
            if n <= 7:
                edges = [(u, v) for u in range(left) for v in range(left, n)]
                actual = sum(components(n, chosen) == 1 for chosen in combinations(edges, n - 1))
                equal(actual, left ** (right - 1) * right ** (left - 1), 'bipartite-spanning-trees')


def tree_checks():
    for n in range(2, 7):
        degree_counts = Counter()
        codes = set()
        for chosen in combinations(pairs(n), n - 1):
            if components(n, chosen) != 1:
                continue
            adj = [set() for _ in range(n)]
            for u, v in chosen:
                adj[u].add(v)
                adj[v].add(u)
            degrees = tuple(map(len, adj))
            degree_counts[degrees] += 1
            equal(degrees.count(1), 2 + sum(d - 2 for d in degrees if d >= 3), 'tree-leaves')
            code = []
            for _ in range(n - 2):
                leaf = next(i for i in range(n) if len(adj[i]) == 1)
                neighbor = adj[leaf].pop()
                code.append(neighbor)
                adj[neighbor].remove(leaf)
            equal(tuple(code.count(i) for i in range(n)), tuple(d - 1 for d in degrees), 'prufer-occurrences')
            codes.add(tuple(code))
            stats['trees-enumerated'] += 1
        equal(sum(degree_counts.values()), n ** (n - 2), 'cayley')
        equal(len(codes), n ** (n - 2), 'prufer-bijection')
        for degrees, actual in degree_counts.items():
            expected = factorial(n - 2) // prod(factorial(d - 1) for d in degrees)
            equal(actual, expected, 'tree-prescribed-degrees')
    for n in range(3, 9):
        edges = [(i, (i + 1) % n) for i in range(n)]
        actual = sum(components(n, chosen) == 1 for chosen in combinations(edges, n - 1))
        equal(actual, n, 'cycle-spanning-trees')


def permutation_checks():
    c = [[0] * 10 for _ in range(10)]
    c[0][0] = 1
    for n in range(1, 9):
        for k in range(1, n + 1):
            c[n][k] = c[n - 1][k - 1] + (n - 1) * c[n - 1][k]
    for n in range(9):
        counts = Counter()
        types = Counter()
        length_totals = Counter()
        for permutation in permutations(range(n)):
            unseen = set(range(n))
            lengths = Counter()
            while unseen:
                v = unseen.pop()
                length = 1
                w = permutation[v]
                while w != v:
                    unseen.remove(w)
                    length += 1
                    w = permutation[w]
                lengths[length] += 1
            counts[sum(lengths.values())] += 1
            types[tuple(lengths[i] for i in range(1, n + 1))] += 1
            length_totals.update(lengths)
            stats['permutations-enumerated'] += 1
        for k in range(n + 1):
            equal(counts[k], c[n][k], 'stirling-first')
        for cycle_type, actual in types.items():
            denominator = prod(i ** a * factorial(a) for i, a in enumerate(cycle_type, 1))
            equal(actual, factorial(n) // denominator, 'permutation-cycle-types')
        for length in range(1, n + 1):
            equal(Fraction(length_totals[length], factorial(n)), Fraction(1, length), 'expected-cycles-by-length')
        equal(Fraction(sum(length_totals.values()), factorial(n)), sum((Fraction(1, i) for i in range(1, n + 1)), Fraction()), 'expected-total-cycles')


def partitions(n):
    """Enumerate set partitions via restricted-growth strings."""
    if n == 0:
        yield ()
        return
    def rec(prefix, largest):
        if len(prefix) == n:
            yield prefix
            return
        for label in range(largest + 2):
            yield from rec(prefix + (label,), max(largest, label))
    yield from rec((0,), 0)


def partition_checks():
    s = [[0] * 10 for _ in range(10)]
    s[0][0] = 1
    for n in range(1, 9):
        for k in range(1, n + 1):
            s[n][k] = s[n - 1][k - 1] + k * s[n - 1][k]
    for n in range(9):
        counts = Counter()
        for partition in partitions(n):
            counts[len(set(partition))] += 1
            stats['partitions-enumerated'] += 1
        for k in range(n + 1):
            equal(counts[k], s[n][k], 'stirling-second')
        equal(sum(counts.values()), sum(s[n]), 'bell')
    for n in range(7):
        for k in range(5):
            actual = sum(len(set(f)) == k for f in product(range(k), repeat=n))
            expected = sum((-1) ** j * comb(k, j) * (k - j) ** n for j in range(k + 1))
            equal(actual, expected, 'onto-inclusion-exclusion')
            equal(actual, factorial(k) * s[n][k], 'onto-stirling')


def phi(n):
    return sum(gcd(i, n) == 1 for i in range(1, n + 1))


def necklace_checks():
    for n in range(1, 9):
        for q in range(1, 4):
            necklaces, bracelets = set(), set()
            for word in product(range(q), repeat=n):
                rotations = [word[i:] + word[:i] for i in range(n)]
                reverse = word[::-1]
                reflections = [reverse[i:] + reverse[:i] for i in range(n)]
                necklaces.add(min(rotations))
                bracelets.add(min(rotations + reflections))
                stats['color-words-enumerated'] += 1
            r = sum(q ** gcd(n, i) for i in range(n))
            divisor_r = sum(phi(d) * q ** (n // d) for d in range(1, n + 1) if n % d == 0)
            equal(r, divisor_r, 'necklace-divisor-sum')
            equal(r, n * len(necklaces), 'necklace-orbits')
            f = n * q ** ((n + 1) // 2) if n % 2 else n // 2 * (q ** (n // 2) + q ** (n // 2 + 1))
            equal(r + f, 2 * n * len(bracelets), 'bracelet-orbits')
            for numerator, denominator, actual in [(r, n, len(necklaces)), (r + f, 2 * n, len(bracelets))]:
                for p in (2, 3, 5, 7):
                    equal((numerator % (p * denominator)) // denominator, actual % p, 'burnside-modular-division')


if __name__ == '__main__':
    graph_checks()
    complete_checks()
    tree_checks()
    permutation_checks()
    partition_checks()
    necklace_checks()
    for name, count in sorted(stats.items()):
        print(f'{name}: {count}')
    print(f'PASS: {sum(count for name, count in stats.items() if not name.endswith("-enumerated"))} checks')
