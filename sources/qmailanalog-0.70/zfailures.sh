echo 'Reasons for failure

One line per reason for delivery failure. Information on each line:
* del is the number of deliveries that ended for this reason.
* xdelay is the total xdelay on those deliveries.
'
( echo del xdelay reason
HOME/bin/failures | sort +2 ) | HOME/bin/columnt | tr _ ' '
