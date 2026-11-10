# Python 3: ints never overflow, Fraction is exact
import sys
from fractions import Fraction
a, b = map(int, sys.stdin.readline().split())
print(a * b, a // b, a % b)   # // and % round down, also < 0
print(pow(a, b, 10**9 + 7))   # a^b mod m, fast
print(Fraction(a, b) + Fraction(1, 6))   # exact, prints p/q
print(str(a ** 50)[:10], len(str(a ** 50)))   # huge numbers
