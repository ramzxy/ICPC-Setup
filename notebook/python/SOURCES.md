# Mathematics references

These sources support the mathematics in the supplement. Basic identities
were derived directly; nontrivial formulas were also checked against small
exhaustive examples. Conditions and conventions are printed beside the formulas.

## Number theory

- [Yale CS467: Euler's theorem and the totient](https://zoo.cs.yale.edu/classes/cs467/2017f/attach/ho11.html):
  totient products, Euler/Fermat conditions and inverses.
- [Gordon College, Number Theory In Context: solving linear congruences](https://math.gordon.edu/ntic/ntic/section-solve-lin-cong.html):
  gcd solvability and residue classes.
- [Stanford: multiplicative order](https://crypto.stanford.edu/pbc/notes/numbertheory/order.html):
  order, divisibility and exponent reduction.
- [Purdue number theory notes](https://www.math.purdue.edu/~twooley/2025nt/2025ntnotes.pdf):
  arithmetic functions, Mobius inversion and totient identities.
- [Purdue: Mobius inversion](https://www.cs.purdue.edu/homes/luo401/cp/mobius-inversion.html):
  coprime-pair sums. The gcd-pair sum follows from sum(phi(d), d|n)=n.
- Noncoprime prime powers follow by writing a=p^s*u with p not dividing u.
  Floor blocks follow from floor(N/i)=q iff N/(q+1)<i<=N/q.

## Graphs and combinatorics

- [MIT LCS technical report 457](https://publications.csail.mit.edu/lcs/pubs/pdf/MIT-LCS-TR-457.pdf):
  cycle-space bases and dimension.
- [Geneseo algebraic graph theory notes](https://www.geneseo.edu/~aguilar/public/assets/courses/310/algebraic-graph-theory.pdf):
  adjacency-matrix walks and traces, tree counts, Matrix--Tree theorem.
- [LSU graph theory notes](https://www.math.lsu.edu/~bogdan/7410/handout.pdf):
  Pruefer sequences and trees with prescribed degrees.
- [Illinois: planar tree--cotree decompositions](https://jeffe.cs.illinois.edu/teaching/comptop/2023/notes/10-planar-tree-cotree.html)
  and [Purdue planar graph notes](https://www.cs.purdue.edu/homes/tamaldey/course/531/planar-graph.pdf):
  Euler's formula, planar edge bounds and cut/cycle structure.
- [NIST DLMF 26.8](https://dlmf.nist.gov/26.8):
  Stirling recurrences (the printed first-kind numbers are unsigned).
- [MIT permutation notes](https://math.mit.edu/~fgotti/docs/Courses/C.%20Combinatorial%20Analysis/11.%20Permutations%20I/Permutations%20I.pdf):
  prescribed cycle types.
- [MIT: Counting Patterns](https://math.mit.edu/~djk/18.310/Lecture-Notes/counting_patterns.html):
  Burnside, necklaces and reflection cases.
- Complete-graph/bipartite cycle counts follow by choosing vertices and dividing
  traversal orders by rotations and reversal. The four-cycle common-neighbor
  sum counts each opposite pair twice.

## Sums, binomials and probability

- [MIT Mathematics for Computer Science](https://ocw.mit.edu/courses/6-042j-mathematics-for-computer-science-spring-2015/mit6_042js15_textbook.pdf):
  generating functions, binomial identities and probability.
- [BYU recursion notes](https://emc2.byu.edu/math495R/Lab08_Recursion/02_branching.html):
  Fibonacci doubling.
- [Utah Putnam notes](https://www.math.utah.edu/~bestvina/putnam/notes.pdf):
  Lucas's theorem.
- [Legendre valuation research note](https://arxiv.org/abs/1907.11902):
  factorial prime valuations.
- [Cornell probability notes](https://www.cs.cornell.edu/courses/cs2800/2014fa/handouts/cameron_prob_notes.pdf):
  variance, covariance, distributions and bounds.
- [Tail-moment identity](https://math.colgate.edu/~integers/x88/x88.pdf):
  first and second moments as weighted tail sums.
- Pair-sum formulas follow by expanding squares or collecting each sorted
  element's coefficient. The unit-factorial binomial formula separates all
  prime factors before division. Exact division modulo m follows from
  A=d*q and reduction modulo d*m.
