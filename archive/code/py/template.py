import os
import sys
input = sys.stdin.buffer.readline
LOCAL = os.environ.get("LOCAL") == "1"

def dbg(*args):
    if LOCAL:
        print(*args, file=sys.stderr)

def solve():
    n = int(input())
    print(n)

if __name__ == "__main__":
    solve()
