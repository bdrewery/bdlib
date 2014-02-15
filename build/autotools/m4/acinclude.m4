AC_DEFUN([MY_ARG_WITH], [
  AC_ARG_WITH(m4_translit([[$1]], [_], [-]),
     [AS_HELP_STRING([--with-m4_translit([$1], [_], [-])],
                     [use $1 (default is $2)])],
     [with_[]$1=$withval],
     [with_[]$1=$2])
])

AC_DEFUN([MY_ARG_WITH_PATH], [
  AC_ARG_WITH(m4_translit([[$1]], [_], [-]),
     [AS_HELP_STRING([--with-m4_translit([$1], [_], [-])=PATH],
                     [$2])],
     [with_[]$1_path=$withval],
     [with_[]$1_path=$3])
])

AC_DEFUN([MY_ARG_ENABLE], [
  AC_MSG_CHECKING([whether $2 is enabled])
  AC_ARG_ENABLE(m4_translit([[$1]], [_], [-]),
     [AS_HELP_STRING([--enable-m4_translit([$1], [_], [-])],
                     [enable $2 support])],
     [
       enable_[]$1=$enableval
       AC_MSG_RESULT([$enableval])
       AC_DEFINE(m4_translit([[USE_$1]], [a-z], [A-Z]), 1, [Define if you want $2 support])
     ],
     [
       # default if not given is DISABLED
       if test "x$enableval" = "x"; then
         enableval="no"
       fi
       enable_[]$1=$enableval
       AC_MSG_RESULT([$enableval])
     ]
  )
])

AC_DEFUN([MY_ARG_DISABLE], [
  AC_MSG_CHECKING([whether $2 is enabled])
  AC_ARG_ENABLE(m4_translit([[$1]], [_], [-]),
     [AS_HELP_STRING([--disable-m4_translit([$1], [_], [-])],
                     [disable $2 support])],
     [
       enable_[]$1=$enableval
       AC_MSG_RESULT([$enableval])
     ],
     [
       # default if not given is ENABLED
       if test "x$enableval" = "x"; then
         enableval="yes"
       fi
       enable_[]$1=$enableval
       AC_MSG_RESULT([$enableval])
       AC_DEFINE(m4_translit([[USE_$1]], [a-z], [A-Z]), 1, [Define if you want $2 support])
     ]
  )
])

dnl EGG_PROG_HEAD_1()
dnl
AC_DEFUN([EGG_PROG_HEAD_1],
[
cat << 'EOF' > conftest.head
a
b
c
EOF

for ac_prog in 'head -1' 'head -n 1' 'sed 1q';
do
  AC_MSG_CHECKING([whether $ac_prog works])
  AC_CACHE_VAL(ac_cv_prog_HEAD_1,
[  if test -n "$HEAD_1"
  then
    ac_cv_prog_HEAD_1="$HEAD_1" # Let the user override the test.
  else
   if test "`cat conftest.head | $ac_prog`" = "a";
   then
     AC_MSG_RESULT([yes])
     ac_cv_prog_HEAD_1=$ac_prog
   else
     AC_MSG_RESULT([no])
   fi
  fi])dnl
  test -n "$ac_cv_prog_HEAD_1" && break
done

if test "${ac_cv_prog_HEAD_1-x}" = "x"
then
  cat << 'EOF' >&2
configure: error:

  This system seems to lack a working 'head -1' or 'head -n 1' command.
  A working 'head -1' (or equivalent) command is required to compile Wraith.

EOF
  exit 1
fi

rm -f conftest.head
HEAD_1=$ac_cv_prog_HEAD_1
AC_SUBST(HEAD_1)dnl
])

