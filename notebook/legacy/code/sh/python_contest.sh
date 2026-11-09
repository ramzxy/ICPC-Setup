# Save template as ~/template.py, paste these functions.
# mkp a: create; cpy a: check; rp a [1]: run [a/1.in].
mkp() { mkdir -p "$1" && cp -n ~/template.py "$1/$1.py"; }
cpy() {
    python3 -c 'import sys
p = sys.argv[1]
compile(open(p, "rb").read(), p, "exec")' "$1/$1.py"
}
rp() {
    cpy "$1" || return
    if [ -n "${2:-}" ]; then
        LOCAL=1 python3 "$1/$1.py" < "$1/$2.in"
    else LOCAL=1 python3 "$1/$1.py"
    fi
}
