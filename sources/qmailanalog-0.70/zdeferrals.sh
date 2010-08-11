echo 'Reasons for deferral

One line per reason for deferral. Information on each line:
* del is the number of deliveries that ended for this reason.
* xdelay is the total xdelay on those deliveries.
'
( echo del xdelay reason
HOME/bin/deferrals | sort +2 ) | HOME/bin/columnt | tr _ ' '
