echo 'Sender uids

One line per sender uid. Information on each line:
* mess is the number of messages sent by this uid.
* bytes is the number of bytes sent by this uid.
* sbytes is the number of bytes successfully received from this uid.
* rbytes is the number of bytes from this uid, weighted by recipient.
* recips is the number of recipients (success plus failure).
* tries is the number of delivery attempts (success, failure, deferral).
* xdelay is the total xdelay incurred by this uid.
'
( echo mess bytes sbytes rbytes recips tries xdelay uid
HOME/bin/suids | sort -n +7 ) | HOME/bin/columnt
