#! /bin/sh

FILE="$1"
TMPNAM="$1.tmp"

SED=$(which gsed 2> /dev/null)
if [ $? -eq 1 ]; then
  SED=$(which sed 2> /dev/null)
fi

if $($SED $FILE \
-e "s/^cache_file=\/dev\/null/cache_file=\.\/build\/config.cache/" \
-e "s/&& echo \"updating cache .cache_file\"//" \
-e "s/confdefs\.h/build\/confdefs\.h/" \
> $TMPNAM); then
  mv -f $TMPNAM $1
fi

