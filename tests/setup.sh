#!/usr/bin/env bash
# Pass a Python executable as argument 1 when python3 is unavailable.
# Private HOME and workspaces; no real startup files are changed.
set -euo pipefail
test_repo=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
test_parent=$(cd -- "${TMPDIR:-/tmp}" && pwd -P)
test_dir=$(mktemp -d "$test_parent/icpc-setup.XXXXXX")
case "$test_dir" in "$test_parent"/icpc-setup.*) ;; *) exit 1 ;; esac
trap 'rm -rf -- "$test_dir"' EXIT
export HOME="$test_dir/private home"
mkdir -p "$HOME"
printf 'bash startup sentinel\n' > "$HOME/.bashrc"
printf 'zsh startup sentinel\n' > "$HOME/.zshrc"
test_startup=$(cksum "$HOME/.bashrc" "$HOME/.zshrc")
export PYTHON=${1:-python3}
test_real_cxx=$(command -v g++ || true)
test_workspace="$test_dir/workspace with spaces"

# Executing installs files, but cannot define functions in this parent shell.
bash "$test_repo/setup.sh" "$test_workspace" > "$test_dir/install-output"
! declare -F tmpl >/dev/null
for test_file in template.py template.cpp; do
    test -f "$test_workspace/tmpl/$test_file"
done
test "$(find "$test_workspace/tmpl" -type f | wc -l)" -eq 2
grep 'source ' "$test_dir/install-output" >/dev/null

# Sourcing installs and defines commands, without changing shell options/cwd.
test_options=$-
test_cwd=$PWD
source "$test_repo/setup.sh" "$test_workspace" >/dev/null
test "$test_options" = "$-"
test "$test_cwd" = "$PWD"
for test_command in tmpl rc rp; do declare -F "$test_command" >/dev/null; done
cd "$test_dir"
tmpl c
for test_letter in A B C; do
    test -f "$test_workspace/$test_letter/$test_letter.py"
    test -f "$test_workspace/$test_letter/$test_letter.cpp"
done
test ! -d "$test_workspace/D"
printf '# keep my solution\n' >> "$test_workspace/A/A.py"
printf '// keep my solution\n' >> "$test_workspace/A/A.cpp"
test_solutions=$(cksum "$test_workspace/A/A.py" "$test_workspace/A/A.cpp")
tmpl
test "$test_solutions" = "$(cksum "$test_workspace/A/A.py" "$test_workspace/A/A.cpp")"
for test_letter in {A..Z}; do
    test -f "$test_workspace/$test_letter/$test_letter.py"
    test -f "$test_workspace/$test_letter/$test_letter.cpp"
done
test ! -e "$test_dir/A"
if tmpl AA 2>/dev/null; then exit 1; fi
if tmpl '' 2>/dev/null; then exit 1; fi
if rp ../outside 2>/dev/null; then exit 1; fi
if rc AA 2>/dev/null; then exit 1; fi
if rp 2>/dev/null; then exit 1; fi

# Normal input() still gives str; paths are literal, not sample numbers.
test "$(printf '42\n' | rp a)" = 42
printf '43\n' > "$test_workspace/A/1.in"
printf '44\n' > "$test_workspace/1"
printf '45\n' > "$test_workspace/sample with spaces.in"
test "$(rp A A/1.in)" = 43
test "$(rp a 1)" = 44
test "$(rp a 'sample with spaces.in')" = 45
test "$(rp A "$test_workspace/A/1.in")" = 43
test "$(rp a < "$test_workspace/A/1.in")" = 43
if rp a missing.in 2>/dev/null; then exit 1; fi
test "$PWD" = "$test_dir"
cat > "$test_workspace/A/A.py" <<'PY'
import os
import sys
s = input()
print(type(s).__name__ + ':' + s)
if os.environ.get('LOCAL') == '1':
    print('python debug', file=sys.stderr)
PY
printf 'hello\n' | rp a > "$test_dir/out" 2> "$test_dir/err"
test "$(cat "$test_dir/out")" = str:hello
test "$(cat "$test_dir/err")" = 'python debug'
printf 'hello\n' | env -u LOCAL "$PYTHON" "$test_workspace/A/A.py" \
    > "$test_dir/out" 2> "$test_dir/err"
test ! -s "$test_dir/err"
printf 'invalid syntax !!!\n' > "$test_workspace/A/A.py"
if rp a </dev/null > "$test_dir/out" 2> "$test_dir/err"; then exit 1; fi
test ! -s "$test_dir/out"

# Stub compiler checks dispatch/flags and prevents running a stale executable.
export COMPILER_LOG="$test_dir/compiler.log"
export CXX="$test_dir/fake compiler"
cat > "$CXX" <<'SH'
#!/usr/bin/env bash
printf '%s\n' "$@" > "$COMPILER_LOG"
if [ "${STUB_FAIL:-0}" = 1 ]; then exit 17; fi
while [ "$#" -gt 0 ]; do
    if [ "$1" = -o ]; then shift; output=$1; fi
    shift
done
cat > "$output" <<'RUN'
#!/usr/bin/env bash
read -r value
printf 'cpp:%s\n' "$value"
printf 'cpp debug\n' >&2
RUN
chmod +x "$output"
SH
chmod +x "$CXX"
rc a A/1.in > "$test_dir/out" 2> "$test_dir/err"
test "$(cat "$test_dir/out")" = cpp:43
test "$(cat "$test_dir/err")" = 'cpp debug'
grep -Fx -- '-DLOCAL' "$COMPILER_LOG" >/dev/null
grep -Fx -- '-std=c++17' "$COMPILER_LOG" >/dev/null
grep -Fx -- 'A/A.cpp' "$COMPILER_LOG" >/dev/null
export STUB_FAIL=1
if rc a </dev/null > "$test_dir/out" 2> "$test_dir/err"; then exit 1; fi
test ! -s "$test_dir/out"
unset STUB_FAIL

if [ -n "$test_real_cxx" ]; then
    CXX=$test_real_cxx
    printf '31\n' | rc B > "$test_dir/out" 2> "$test_dir/err"
    test "$(cat "$test_dir/out")" = 31
    test ! -s "$test_dir/err"
    sed 's/cout << n/dbg(n); cout << n/' "$test_workspace/B/B.cpp" > "$test_dir/debug.cpp"
    cp "$test_dir/debug.cpp" "$test_workspace/B/B.cpp"
    printf '32\n' | rc b > "$test_dir/out" 2> "$test_dir/err"
    test "$(cat "$test_dir/out")" = 32
    grep -Fx 'n = 32' "$test_dir/err" >/dev/null
fi

# Installer reruns preserve all template edits and never touch HOME/rc files.
printf '# user marker\n' >> "$test_workspace/tmpl/template.py"
printf '// user marker\n' >> "$test_workspace/tmpl/template.cpp"
test_templates=$(cksum "$test_workspace"/tmpl/*)
source "$test_repo/setup.sh" "$test_workspace" >/dev/null
test "$test_templates" = "$(cksum "$test_workspace"/tmpl/*)"
test "$test_startup" = "$(cksum "$HOME/.bashrc" "$HOME/.zshrc")"
test "$(find "$HOME" -type f | wc -l)" -eq 2

# The printed command block works alone after the templates have been entered.
mkdir "$test_dir/second"
cd "$test_dir/second"
bash "$test_repo/setup.sh" >/dev/null
sed -n '/^# BEGIN CONTEST COMMANDS$/,/^# END CONTEST COMMANDS$/p' \
    "$test_repo/setup.sh" > "$test_dir/printed-block"
source "$test_dir/printed-block"
tmpl a
test -f A/A.py
test -f A/A.cpp
test ! -e B
test "$(printf '46\n' | rp A)" = 46
printf 'Passed local install/source, tmpl ranges, input paths, and Python/debug checks.\n'
if [ -n "$test_real_cxx" ]; then
    printf 'Passed real C++ compile/run/debug and stale-executable checks.\n'
else printf 'C++ dispatch tested with stub; real compilation needs g++.\n'
fi
