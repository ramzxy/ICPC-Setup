#!/usr/bin/env bash
# Practice: source /path/to/ICPC-Setup/setup.sh [workspace]
# At the contest, type the templates and the marked command block.

# BEGIN CONTEST COMMANDS
tmpl() {
    local p
    for p in {A..N}; do
        cp -rn tmpl/. "$p"
    done
}
_c() { g++ -std=c++17 -O2 "$1/cpp.cpp" -o "$1/$1"; }
rc() {
    local p=${1^^} input=/dev/stdin
    [ "$#" -gt 1 ] && input="$p/$2.in"
    _c "$p" && "./$p/$p" < "$input"
}
rp() {
    local p=${1^^} input=/dev/stdin
    [ "$#" -gt 1 ] && input="$p/$2.in"
    python3 "$p/py.py" < "$input"
}
_t() (
    set -o pipefail
    shopt -s failglob
    bad=0
    for f in "$1"/*.in; do
        echo "$f"
        "${@:2}" < "$f" | diff -bB "${f%.in}.out" - &&
            echo OK || { echo FAIL; bad=1; }
    done
    exit "$bad"
)
tp() { _t "${1^^}" rp "$1"; }
tc() {
    local p=${1^^}
    _c "$p" && _t "$p" "./$p/$p"
}
# END CONTEST COMMANDS

# Installer for practice machines. No exits, shell-option changes, or rc edits.
_icpc_install() {
    local setup_source setup_target setup_file
    if [ "$#" -gt 1 ] || { [ "$#" -eq 1 ] && [ -z "$1" ]; }; then
        printf 'Usage: source setup.sh [workspace]\n' >&2; return 2
    fi
    if [ -n "${BASH_VERSION:-}" ]; then
        setup_source=${BASH_SOURCE[0]}
    else printf 'Use Bash.\n' >&2; return 2
    fi
    setup_source=$(cd -- "$(dirname -- "$setup_source")" && pwd) || return
    setup_target=${1:-.}
    mkdir -p -- "$setup_target/tmpl" || return
    setup_target=$(cd -- "$setup_target" && pwd) || return
    for setup_file in py.py cpp.cpp; do
        if [ -e "$setup_target/tmpl/$setup_file" ] || [ -L "$setup_target/tmpl/$setup_file" ]; then
            printf 'Kept existing tmpl/%s\n' "$setup_file"
        else
            cp "$setup_source/tmpl/$setup_file" "$setup_target/tmpl/$setup_file" || return
            printf 'Created tmpl/%s\n' "$setup_file"
        fi
    done
    printf '\nWorkspace: %s\n' "$setup_target"
    printf 'Enable commands in each terminal with:\nsource %q %q\n' "$setup_source/setup.sh" "$setup_target"
    printf 'cd into that workspace, then: tmpl    rp A 1    rc B 1    tp A    tc B\n'
}
if _icpc_install "$@"; then
    unset -f _icpc_install
else
    unset -f _icpc_install
    false
fi
