#!/bin/bash
# Compatibility entry point: build the unified guide.
set -euo pipefail
exec bash "$(dirname "$0")/../build.sh" "$@"
