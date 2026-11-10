#!/usr/bin/env bash
set -euo pipefail
repo=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
test_parent=$(cd "${TMPDIR:-/tmp}" && pwd -P)
test_dir=$(mktemp -d "$test_parent/icpc-short.XXXXXX")
case "$test_dir" in "$test_parent"/icpc-short.*) ;; *) exit 1 ;; esac
trap 'rm -rf -- "$test_dir"' EXIT
mkdir -p "$test_dir/bin"
export TEST_PYTHON=${1:-python3}
cat > "$test_dir/bin/python3" <<'SH'
#!/usr/bin/env bash
exec "$TEST_PYTHON" "$@"
SH
chmod +x "$test_dir/bin/python3"
export PATH="$test_dir/bin:$PATH"
workspace="$test_dir/workspace with spaces"
bash "$repo/setup.sh" "$workspace" >/dev/null
before=$PWD
options=$-
source "$repo/setup.sh" "$workspace" >/dev/null
test "$before" = "$PWD" && test "$options" = "$-"
cd "$workspace"
mkdir -p tmpl/nested
printf "copied\n" > tmpl/nested/note
tmpl
for letter in {A..N}; do
    test -f "$letter/py.py" && test -f "$letter/cpp.cpp"
    test "$(cat "$letter/nested/note")" = copied
    test ! -e "$letter/tmpl"
done
test ! -e O && test ! -e Z
printf '# keep\n' >> A/py.py
old=$(cksum A/py.py)
tmpl
test "$old" = "$(cksum A/py.py)" && test -f N/cpp.cpp
test ! -e A/tmpl
test "$(printf '42\n' | rp a)" = 42
printf '43\n' > A/1.in
printf '47\n' > 'A/sample input.in'
test "$(rp a 1)" = 43
test "$(rp A 'sample input')" = 47
test "$(rp A < A/1.in)" = 43
cat > A/py.py <<'PY'
import sys
s = input()
print(type(s).__name__ + ':' + s)
print('debug', file=sys.stderr)
PY
printf 'hello\n' | rp a > out 2>err
test "$(cat out)" = str:hello && test "$(cat err)" = debug
# Batch testing keeps the input/expected files intact, ignores whitespace
# amount and blank lines, and rejects different token boundaries.
cat > D/py.py <<'PY'
import sys
s = input()
with open('python-runs', 'a') as log:
    print(s, file=log)
print(s)
print('debug', file=sys.stderr)
if s == 'runtime':
    sys.exit(3)
PY
printf '42\n' > D/1.in
printf '42\n' > D/1.out
printf '1   2\n' > D/2.in
printf '1 2\n\n' > D/2.out
samples=$(cksum D/*.in D/*.out)
tp d > out 2>err
test "$(grep -c '^OK$' out)" -eq 2
grep -q debug err
test "$samples" = "$(cksum D/*.in D/*.out)"

# All later cases must run after a mismatch, a missing answer, and a process
# that prints the expected answer but exits nonzero. Run inside `if` so these
# checks cannot accidentally rely on the caller's `set -e` behavior.
printf '12\n' > D/1.in
printf '1 2\n' > D/1.out
printf 'ok\n' > D/2.in
printf 'ok\n' > D/2.out
printf 'missing\n' > D/3.in
printf 'runtime\n' > D/4.in
printf 'runtime\n' > D/4.out
printf 'after\n' > D/5.in
printf 'after\n' > D/5.out
samples=$(cksum D/*.in D/*.out)
: > python-runs
if tp d > out 2>err; then exit 1; fi
test "$(grep -c '^OK$' out)" -eq 2
test "$(grep -c '^FAIL$' out)" -eq 3
for sample in 1 2 3 4 5; do grep -q "D/$sample.in" out; done
grep -q '^runtime' python-runs
test "$(tail -1 python-runs | tr -d '\r')" = after
test "$samples" = "$(cksum D/*.in D/*.out)"
test ! -e D/3.out
if tp N > out 2>err; then exit 1; fi
shopt -s nullglob
if tp n > out 2>err; then exit 1; fi
shopt -u nullglob

if command -v g++ >/dev/null; then
    test "$(printf '44\n' | rc b)" = 44
    cp A/1.in B/1.in
    cp 'A/sample input.in' 'B/sample input.in'
    test "$(rc b 1)" = 43
    test "$(rc B 'sample input')" = 47
    sed 's/cout << n/dbg(n); cout << n/' B/cpp.cpp > debug.cpp
    cp debug.cpp B/cpp.cpp
    printf '45\n' | rc b > out 2>err
    test "$(cat out)" = 45 && grep -q 'n = 45' err
    # Count real compiler invocations: tc must compile once for the batch.
    export TEST_REAL_CXX=$(command -v g++)
    export TEST_COMPILER_LOG="$test_dir/compiles"
    cat > "$test_dir/bin/g++" <<'SH'
#!/usr/bin/env bash
printf 'compile\n' >> "$TEST_COMPILER_LOG"
exec "$TEST_REAL_CXX" "$@"
SH
    chmod +x "$test_dir/bin/g++"
    hash -r
    cat > E/cpp.cpp <<'CPP'
#include <bits/stdc++.h>
using namespace std;
int main() {
    string s;
    getline(cin, s);
    ofstream("cpp-runs", ios::app) << s << '\n';
    cout << s << '\n';
    return s == "runtime" ? 3 : 0;
}
CPP
    printf '42\n' > E/1.in
    printf '42\n' > E/1.out
    printf '1   2\n' > E/2.in
    printf '1 2\n\n' > E/2.out
    tc e > out 2>err
    test "$(grep -c '^OK$' out)" -eq 2
    test "$(wc -l < "$TEST_COMPILER_LOG")" -eq 1
    cp D/*.in D/*.out E/
    samples=$(cksum E/*.in E/*.out)
    : > cpp-runs
    if tc e > out 2>err; then exit 1; fi
    test "$(grep -c '^OK$' out)" -eq 2
    test "$(grep -c '^FAIL$' out)" -eq 3
    for sample in 1 2 3 4 5; do grep -q "E/$sample.in" out; done
    grep -q '^runtime' cpp-runs
    test "$(tail -1 cpp-runs | tr -d '\r')" = after
    test "$samples" = "$(cksum E/*.in E/*.out)"
    test ! -e E/3.out
    test "$(wc -l < "$TEST_COMPILER_LOG")" -eq 2
    printf '#!/usr/bin/env bash\nexit 17\n' > "$test_dir/bin/g++"
    chmod +x "$test_dir/bin/g++"
    hash -r
    if rc B </dev/null > out 2>err; then exit 1; fi
    test ! -s out
    if tc B </dev/null > out 2>err; then exit 1; fi
    test ! -s out
fi
# The paper block is self-contained, with no installer state.
sed -n '/^# BEGIN CONTEST COMMANDS$/,/^# END CONTEST COMMANDS$/p' \
    "$repo/setup.sh" > "$test_dir/block"
source "$test_dir/block"
tmpl
printf '46\n' | rp C | grep -qx 46
printf '46\n' > C/1.in
printf '46\n' > C/1.out
tp c | grep -qx OK
printf 'Short setup: generation, preservation, stdin/files, str/debug, batch testing and C++ checks passed.\n'
