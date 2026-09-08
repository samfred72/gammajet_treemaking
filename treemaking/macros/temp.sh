set -euo pipefail

if [ $# -lt 2 ]; then
  echo "Usage: $0 <output_file> <file_or_glob> [<file_or_glob> ...]" >&2
  exit 1
fi

OUTPUT="$1"
shift

TODAY=$(date +%Y-%m-%d)
SELECTED=()

for f in "$@"; do
  [ -f "$f" ] || continue

  mdate=$(date -r "$f" +%Y-%m-%d)
  if [ "$mdate" != "$TODAY" ]; then
    continue
  fi

  if lsof -Fa -- "$f" 2>/dev/null | grep -qE '^a[wu]'; then
    echo "Skipping (open for writing): $f" >&2
    continue
  fi

  SELECTED+=("$f")
done

if [ ${#SELECTED[@]} -eq 0 ]; then
  echo "No eligible files found (modified today, not open for writing)." >&2
  exit 1
fi

echo "Combining ${#SELECTED[@]} file(s) into $OUTPUT:" >&2
printf '  %s\n' "${SELECTED[@]}" >&2

# ---- COMBINE ----
hadd -f "$OUTPUT" "${SELECTED[@]}"
# For plain-file concatenation instead, replace the line above with:
#   cat "${SELECTED[@]}" > "$OUTPUT"
