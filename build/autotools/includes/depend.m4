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
  GCC3="-W -std=c++98 -Wabi -Wextra -Wno-unused-parameter -Wdisabled-optimization -Wmissing-format-attribute"
  DEBGCC3=""
  DEBNOUNITSGCC3="-Weffc++"
fi
AC_SUBST(CCDEPMODE)dnl
AC_SUBST(GCC3)dnl
AC_SUBST(DEBGCC3)dnl
AC_SUBST(DEBNOUNITSGCC3)dnl
])

AC_DEFUN([DO_DEPS],
[
files="src/Makefile.in build/Makefile.in src/tests/Makefile.in"
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
    base=`$BASENAME $file .o`
    if test -f "$dirpart/$base.cpp"; then
      ext="cpp"
    elif test -f "$dirpart/$base.c"; then
      ext="c"
    else
      continue
    fi
    if ! test -f "$dirpart/.deps/$base.Po"; then
      echo '# dummy' > "$dirpart/.deps/$base.Po"
      #Remove the .o file, because it needs to be recompiled for its dependancies.
      if test -f "$dirpart/$base.o"; then
        rm -f "$dirpart/$base.o"
      fi
    fi
    echo "include .deps/$base.Po" >> "$dirpart/.deps/includes"
    echo "$base.$ext:" >> "$dirpart/.deps/includes"
  done
done
])


