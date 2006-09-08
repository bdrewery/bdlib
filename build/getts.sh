#! /bin/sh

rm -f ts ts.exe
gcc -o ts build/ts.c > /dev/null 2>&1
date=$(build/getdate.sh)
./ts $date
rm -f ts ts.exe
