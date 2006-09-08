# DO_DEPS
# ------------------------------


dnl  CHECK_DEPMODE()
dnl
AC_DEFUN([CHECK_DEPMODE],
[
CCDEPMODE=gcc
num=`$CXX -dumpversion | sed "s/^\\\(.\\\).*/\\\1/"`
if test $num -ge "3"; then
  CCDEPMODE=gcc3
  GCC3="-W -Wno-unused-parameter -Wdisabled-optimization -Wmissing-format-attribute"
fi
AC_SUBST(CCDEPMODE)dnl
AC_SUBST(GCC3)dnl
])

AC_DEFUN([DO_DEPS],
[
files="src/Makefile.in lib/bdlib/Makefile.in lib/wraith/Makefile.in build/Makefile.in"
for mf in $files; do
  # Strip MF so we end up with the name of the file.
#  echo "MF: $mf"
  mf=`echo "$mf" | sed -e 's/:.*$//'`
  dirpart=`AS_DIRNAME("$mf")`
#  echo "dirpart: $dirpart mf: $mf"
#  rm -rf "$dirpart/.deps/"
  rm -f "$dirpart/.deps/includes"
  test -d "$dirpart/.deps" || mkdir "$dirpart/.deps"
  touch "$dirpart/.deps/includes"
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
    test -f "$dirpart/$base.c" || continue
    if ! test -f "$dirpart/.deps/$base.Po"; then
      echo '# dummy' > "$dirpart/.deps/$base.Po"
      #Remove the .o file, because it needs to be recompiled for its dependancies.
      if test -f "$dirpart/$base.o"; then
        rm -f "$dirpart/$base.o"
      fi
    fi
    echo "include .deps/$base.Po" >> "$dirpart/.deps/includes"
    echo "_$base.c:" >> "$dirpart/.deps/includes"
  done
done
])


