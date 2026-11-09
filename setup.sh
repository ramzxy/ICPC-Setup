#!/usr/bin/env bash
# Practice: source /path/to/ICPC-Setup/setup.sh [workspace]
# At the contest, type the templates and the marked command block.

# BEGIN CONTEST COMMANDS
CONTEST_ROOT=$PWD
PYTHON=${PYTHON:-python3}
CXX=${CXX:-g++}
unalias tmpl rc rp 2>/dev/null || true
tmpl() {
    [ "$#" -le 1 ] || { printf 'tmpl [last-letter]\n' >&2; return 2; }
    local last=${1-Z} letter ext file
    case "$last" in [a-zA-Z]) ;; *) printf 'Use one letter A-Z.\n' >&2; return 2 ;; esac
    last=$(printf '%s' "$last" | tr '[:lower:]' '[:upper:]')
    for letter in {A..Z}; do
        mkdir -p -- "$CONTEST_ROOT/$letter" || return
        for ext in py cpp; do
            file="$CONTEST_ROOT/$letter/$letter.$ext"
            if [ ! -e "$file" ] && [ ! -L "$file" ]; then
                cp "$CONTEST_ROOT/tmpl/template.$ext" "$file" || return
            fi
        done
        [ "$letter" = "$last" ] && break
    done
}
rc() (
    [ "$#" -ge 1 ] && [ "$#" -le 2 ] || { printf 'rc letter [input-file]\n' >&2; return 2; }
    case "$1" in [a-zA-Z]) ;; *) printf 'Use one letter A-Z.\n' >&2; return 2 ;; esac
    local letter
    letter=$(printf '%s' "$1" | tr '[:lower:]' '[:upper:]')
    cd -- "$CONTEST_ROOT" || return
    "$CXX" -std=c++17 -O2 -Wall -Wextra -g -DLOCAL \
        "$letter/$letter.cpp" -o "$letter/$letter" || return
    if [ "$#" -eq 2 ]; then "./$letter/$letter" < "$2"
    else "./$letter/$letter"
    fi
)
rp() (
    [ "$#" -ge 1 ] && [ "$#" -le 2 ] || { printf 'rp letter [input-file]\n' >&2; return 2; }
    case "$1" in [a-zA-Z]) ;; *) printf 'Use one letter A-Z.\n' >&2; return 2 ;; esac
    local letter
    letter=$(printf '%s' "$1" | tr '[:lower:]' '[:upper:]')
    cd -- "$CONTEST_ROOT" || return
    if [ "$#" -eq 2 ]; then LOCAL=1 "$PYTHON" "$letter/$letter.py" < "$2"
    else LOCAL=1 "$PYTHON" "$letter/$letter.py"
    fi
)
# END CONTEST COMMANDS

# Installer for practice machines. No exits, shell-option changes, or rc edits.
_icpc_install() {
    local setup_source setup_target setup_file
    if [ "$#" -gt 1 ] || { [ "$#" -eq 1 ] && [ -z "$1" ]; }; then
        printf 'Usage: source setup.sh [workspace]\n' >&2; return 2
    fi
    if [ -n "${BASH_VERSION:-}" ]; then
        setup_source=${BASH_SOURCE[0]}
    elif [ -n "${ZSH_VERSION:-}" ]; then
        setup_source=${(%):-%x}
    else printf 'Use Bash or zsh.\n' >&2; return 2
    fi
    setup_source=$(cd -- "$(dirname -- "$setup_source")" && pwd) || return
    setup_target=${1:-.}
    mkdir -p -- "$setup_target/tmpl" || return
    setup_target=$(cd -- "$setup_target" && pwd) || return
    for setup_file in template.py template.cpp; do
        if [ -e "$setup_target/tmpl/$setup_file" ] || [ -L "$setup_target/tmpl/$setup_file" ]; then
            printf 'Kept existing tmpl/%s\n' "$setup_file"
        else
            cp "$setup_source/tmpl/$setup_file" "$setup_target/tmpl/$setup_file" || return
            printf 'Created tmpl/%s\n' "$setup_file"
        fi
    done
    CONTEST_ROOT=$setup_target
    printf '\nWorkspace: %s\n' "$CONTEST_ROOT"
    printf 'Enable commands in each terminal with:\nsource %q %q\n' "$setup_source/setup.sh" "$CONTEST_ROOT"
    printf 'Then: tmpl C    rp a A/1.in    rc b B/1.in\n'
}
if _icpc_install "$@"; then
    unset -f _icpc_install
else
    unset -f _icpc_install
    false
fi
