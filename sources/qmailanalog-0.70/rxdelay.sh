
awk '
  {
    str = sprintf("%.2f",$4/$3)
    print str,$3,$5
  }
' | sort -n
