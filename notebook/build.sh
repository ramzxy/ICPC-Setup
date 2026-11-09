#!/bin/bash
# Build the unified NWERC guide (TeX Live or MiKTeX with latexmk).
# Usage: bash notebook/build.sh [output-directory]
set -euo pipefail
cd "$(dirname "$0")"
out=${1:-.}
mkdir -p .build "$out"
latexmk -pdf -interaction=nonstopmode -halt-on-error -quiet \
    -outdir=.build notebook.tex
cp .build/notebook.pdf "$out/notebook.pdf"
