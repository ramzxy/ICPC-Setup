#!/bin/bash
# Builds notebook.pdf. Needs TeX Live (pdflatex and latexmk).
# Usage: ./build.sh            writes notebook.pdf here
#        ./build.sh some/dir   writes some/dir/notebook.pdf instead
set -euo pipefail
cd "$(dirname "$0")"
out=${1:-.}
latexmk -pdf -interaction=nonstopmode -halt-on-error -quiet -outdir="$out" notebook.tex
latexmk -c -outdir="$out" notebook.tex   # remove helper files, keep the PDF
