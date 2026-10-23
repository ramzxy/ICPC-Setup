#!/bin/bash
# Builds a PDF of one section only, to check its layout. Prints the PDF's path.
# Usage: ./preview.sh dp     (for sections/dp.tex)
set -euo pipefail
cd "$(dirname "$0")"
out=$(mktemp -d)
if ! pdflatex -interaction=nonstopmode -halt-on-error -output-directory="$out" \
        -jobname="preview-$1" "\def\onlysection{$1}\input{notebook}" >/dev/null; then
    grep -A6 '^!' "$out/preview-$1.log" || tail -20 "$out/preview-$1.log"
    exit 1
fi
echo "$out/preview-$1.pdf"
