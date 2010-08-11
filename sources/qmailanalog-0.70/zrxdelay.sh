echo 'Recipients in the best order for mailing lists

One line per recipient, sorted by avg. Information on each line:
* avg is the _average_ xdelay for the recipient.
* tries is the number of deliveries that avg is based on.
'
( echo avg tries recipient
HOME/bin/recipients | HOME/bin/rxdelay ) | HOME/bin/columnt
