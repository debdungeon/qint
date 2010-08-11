dependon auto-str conf-home
formake './auto-str auto_home `head -1 conf-home` > auto_home.c'
./auto-str auto_home `head -1 conf-home`
