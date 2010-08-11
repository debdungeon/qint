echo 'Reasons for success

One line per reason for successful delivery. Information on each line:
* del is the number of deliveries that ended for this reason.
* xdelay is the total xdelay on those deliveries.
'
( echo del xdelay reason
HOME/bin/successes | sort +2 ) | HOME/bin/columnt | tr _ ' '
