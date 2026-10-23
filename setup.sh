#!/bin/bash
# Sets up a practice machine the way we set up the contest machine:
#   - copies the contest template to ~/template.cpp
#   - adds the mk / c / r commands (notebook page 2) to your shell
# Run once with  bash setup.sh  and then open a new terminal.
# Running it again is safe: it replaces its own block in the rc file.
set -euo pipefail
cd "$(dirname "$0")"

case "$(basename "${SHELL:-bash}")" in
    zsh) rc="$HOME/.zshrc" ;;
    *)   rc="$HOME/.bashrc" ;;
esac

# macOS: plain g++ is Apple clang (no <bits/stdc++.h>); use Homebrew GCC.
cxx=g++
for v in 20 19 18 17 16 15 14 13; do
    if command -v "g++-$v" >/dev/null 2>&1; then cxx="g++-$v"; break; fi
done

cp notebook/code/cpp/template.cpp "$HOME/template.cpp"

begin="# >>> ICPC contest setup >>>"
end="# <<< ICPC contest setup <<<"
touch "$rc"
cp "$rc" "$rc.before-icpc-setup"   # a backup, just in case
sed "/^$begin\$/,/^$end\$/d" "$rc.before-icpc-setup" > "$rc"
{
    echo "$begin"
    echo "unalias mk c r 2>/dev/null   # old aliases with the same names"
    grep -v '^#' notebook/code/sh/contest.sh | sed "s/g++ /$cxx /"
    echo "$end"
} >> "$rc"

echo "Added mk, c and r to $rc (compiler: $cxx)."
echo "Template: ~/template.cpp. Open a new terminal or run: source $rc"
