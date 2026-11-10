# Previous BAPC material

Historical print snapshots and source documents live here. Use
[the current NWERC guide](../notebook/README.md) for preparation and printing.
The setup commands in these archived documents describe the old workflow.

- `bapc-2026.pdf`: original C++ notebook, including its old notes sheet.
- `python-supplement-2026.pdf`: Python/mathematics supplement and cover.
- `bapc-combined-2026.pdf`: earlier combined BAPC print copy.
- `bapc.tex`, `sections/`: original C++ notebook sources.
- `python/`: former standalone supplement sources, including its historical math.
- `code/`: historical setup helpers and Python template required by those sources.

The C++ notebook reads shared implementations from `../reference/` rather than
keeping another copy. Source paths have been adjusted to this directory layout;
the historical PDFs are preserved unchanged. Math source snapshots in `python/`
belong to those old documents; current math edits go in `../notebook/sections/math/`.

To preview an old C++ chapter from the repository root:

```sh
bash archive/preview.sh dp
```

To rebuild the original C++ source, run `latexmk -pdf -outdir=.build bapc.tex`
from this directory. To rebuild the standalone supplement, run
`latexmk -pdf -outdir=.build main.tex` from `python/`.
`python/combined.tex` assembles the saved historical PDFs.
