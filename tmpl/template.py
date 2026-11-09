import os
import sys

# Optional, only if needed: input = sys.stdin.readline
# Normal input() returns a string and removes the trailing newline.
LOCAL = os.environ.get("LOCAL") == "1"


def dbg(*args):
    if LOCAL:
        print(*args, file=sys.stderr)


def solve():
    n = int(input())
    print(n)


if __name__ == "__main__":
    solve()
