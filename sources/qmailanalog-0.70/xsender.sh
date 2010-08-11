
awk '
  /^d/ {
    if ($7 == x) print
  }
  /^m/ {
    if ($8 == x) print
  }
' x="<$1>"
