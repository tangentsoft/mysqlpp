#!/bin/sh

libtoolize --automake
aclocal || aclocal -I macros

autoconf
autoheader

for fn in AUTHORS ChangeLog NEWS README
do
  if [ ! -e  "$fn" ]
  then
    touch "$fn"
  fi
done
  
automake -a
./configure $*

