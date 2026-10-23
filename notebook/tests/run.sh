#!/bin/bash
# Compiles every notebook snippet and checks it against brute force.
# Usage: tests/run.sh          run every tests/test_*.cpp and the template
#        tests/run.sh graphs   run only tests/test_graphs.cpp
set -euo pipefail
cd "$(dirname "$0")"

# macOS g++ is Apple clang (no <bits/stdc++.h>); prefer real GCC from Homebrew.
CXX=g++
for v in 20 19 18 17 16 15 14 13; do
    if command -v "g++-$v" >/dev/null 2>&1; then CXX="g++-$v"; break; fi
done
mkdir -p build

# template.cpp without main(). Written with mv so a run in parallel never
# reads half a file.
sed '/^int main/,$d' ../code/cpp/template.cpp > "build/prelude.$$"
mv "build/prelude.$$" build/prelude.hpp

if [ $# -gt 0 ]; then tests=("test_$1.cpp"); else tests=(test_*.cpp); fi
for t in "${tests[@]}"; do
    name=${t%.cpp}
    echo "$name ($CXX)"
    $CXX -std=c++17 -O1 -Wall -Wextra -D_GLIBCXX_DEBUG -I build "$t" -o "build/$name"
    "./build/$name"
done

if [ $# -eq 0 ]; then
    $CXX -std=c++17 -Wall ../code/cpp/template.cpp -o build/template
    [ "$(echo 7 | ./build/template)" = "7" ] || { echo "FAIL: template"; exit 1; }
    $CXX -std=c++17 -Wall -DLOCAL ../code/cpp/template.cpp -o build/template_local
    [ "$(echo 7 | ./build/template_local)" = "7" ] || { echo "FAIL: template -DLOCAL"; exit 1; }
    echo "template OK"
    want=$'60 2 2\n248832\n77/30\n9100438150 54'
    [ "$(echo 12 5 | python3 ../code/py/bignum.py)" = "$want" ] \
        || { echo "FAIL: bignum.py"; exit 1; }
    echo "bignum.py OK"
    echo "All snippets OK"
fi
