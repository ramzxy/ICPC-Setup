import os, sys
# input = sys.stdin.readline  # optional; str, keeps newline
# write = sys.stdout.write    # optional; adds no newline

def dbg(*args):
    if os.getenv("LOCAL"):
        print(*args, file=sys.stderr)

def ints():
    return list(map(int, input().split()))

def solve():
    n = int(input())
    print(n)

solve()
