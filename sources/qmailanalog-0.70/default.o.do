if test -r $2=m
then
  dependon $2=m $2.s
  directtarget
  as -o $1 $2.s
  formake as -o $1 $2.s
  exit 0
fi
depend -$2=m

directtarget
dependon compile
dependcpp $2.c
formake ./compile $2.c
./compile $2.c
