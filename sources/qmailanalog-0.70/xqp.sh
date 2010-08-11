
awk '
  /^d/ {
    if ($9 == x) print
  }
  /^m/ {
    if ($9 == x) print
  }
' x="$1"
