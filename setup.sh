#!/bin/bash
# ===== ICPC Setup for bash/zsh =====

PROFILE=""
if [ -n "$ZSH_VERSION" ]; then
    PROFILE="$HOME/.zshrc"
elif [ -n "$BASH_VERSION" ]; then
    PROFILE="$HOME/.bashrc"
else
    PROFILE="$HOME/.bashrc"
fi

echo "Adding ICPC aliases to $PROFILE ..."

cat << 'EOF' >> $PROFILE

# ====== ICPC Starter Aliases ======
alias c='g++ -Wall -Wconversion -Wfatal-errors -g -std=c++17 -fsanitize=undefined,address'
alias r='./a.out < in.txt > out.txt'
alias chk='diff -w -B out.txt ans.txt'

EOF

echo "Done ✅ — restart your terminal or run: source $PROFILE"

