AC_DEFUN([DO_UNITS],
[
dirs="src/tests/"
for dir in "`find src -type d -name tests`"; do
  dirpart=`AS_DIRNAME("$dir")`
  dirNoSrc="`echo $dir | sed -e 's/src\///'`"

  if test -f "$dir/Makefile.in"; then
    mf="$dir/Makefile.in"
  elif test -f "$dir/Makefile"; then
    mf="$dir/Makefile"
  fi

  for file in `sed -n -e '
    /^OBJS = .*\\\\$/ {
      s/^OBJS = //
      :loop
        s/\\\\$//
        p
        n
        /\\\\$/ b loop
      p
    }
    /^OBJS = / s/^OBJS = //p' < "$mf"`;
  do
    base=`basename $file .o`
    sourceName="`basename $base Test`"
    sourceFile="$dirpart/$sourceName"
    if test -f "$sourceFile.c"; then
      TESTOBJS="$TESTOBJS $sourceName.lo"
      TESTUNITOBJS="$TESTUNITOBJS $dirNoSrc/$base.o"
    fi
  done
done
AC_SUBST(TESTOBJS)dnl
AC_SUBST(TESTUNITOBJS)dnl
])
