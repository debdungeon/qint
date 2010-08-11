
awk '
  /^d/ {
    if ($8 == x) print
  }
' x="$1"
