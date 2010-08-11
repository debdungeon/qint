awk '
  /^d d/ {
    reason = $11
    fail[reason] += 1
    xdelay[reason] += $5 - $4
  }
  END {
    for (reason in fail) {
      str = sprintf("%.2f",xdelay[reason])
      print fail[reason],str,reason
    }
  }
'
