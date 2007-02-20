#! /bin/sh

FILE="$1"
TMPNAM="$1.tmp"

if sed $FILE \
-e "s/^cache_file=\/dev\/null/cache_file=\.\/build\/config.cache/" \
-e "s/&& echo \"updating cache .cache_file\"//" \
-e "s/confdefs\.h/build\/confdefs\.h/" \
> $TMPNAM; then
  mv -f $TMPNAM $1
fi
 
