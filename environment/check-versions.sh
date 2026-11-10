#!/usr/bin/env bash
set -euo pipefail
source /etc/os-release
test "$ID" = ubuntu && test "$VERSION_ID" = 24.04
test "$(g++ -dumpfullversion)" = 13.3.0
pypy3 -c 'import platform, sys; assert platform.python_implementation() == "PyPy"; assert sys.version_info[:3] == (3, 9, 18); assert sys.pypy_version_info[:3] == (7, 3, 15)'
test "$(readlink -f "$(command -v python3)")" = "$(readlink -f "$(command -v pypy3)")"
printf 'OS: %s\n' "$PRETTY_NAME"
g++ --version | head -n 1
pypy3 --version
printf 'Tooling interpreter: '
/usr/bin/python3 --version
