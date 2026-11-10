# Once at the start: save the template as ~/template.cpp, paste this
# into the terminal (or put it in ~/.bashrc). Then for problem A:
#   mk a     folder a/ with a/a.cpp copied from the template
#   c a      compile a/a.cpp: dbg on, checks for out-of-range
#   r a      compile and run: paste the input, then Ctrl-D
#   r a 1    compile and run on a/1.in
mk() { mkdir -p "$1" && cp -n ~/template.cpp "$1/$1.cpp"; }
c() {
    g++ -std=c++17 -O2 -Wall -g -DLOCAL \
        -fsanitize=address,undefined "$1/$1.cpp" -o "$1/$1"
}
r() {
    c "$1" || return
    if [ -n "$2" ]; then ./"$1/$1" < "$1/$2.in"
    else ./"$1/$1"
    fi
}
