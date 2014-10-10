dnl aclocal.m4
dnl   macros autoconf uses when building configure from configure.in
dnl
dnl $Id: acinclude.m4 2911 2006-07-11 00:32:12Z bryan $
dnl

dnl EGG_CHECK_CCPIPE()
dnl
dnl This function checks whether or not the compiler supports the `-pipe' flag,
dnl which speeds up the compilation.
dnl
AC_DEFUN([EGG_CHECK_CCPIPE],
[
  if test -n "$GXX" && test -z "$no_pipe"; then
    AC_CACHE_CHECK([whether the compiler understands -pipe], egg_cv_var_ccpipe, [
        ac_old_CXX="$CXX"
        CXX="$CXX -pipe"
        AC_COMPILE_IFELSE([AC_LANG_SOURCE([
          int main ()
          {
            return(0);
          }
        ])], [
          egg_cv_var_ccpipe="yes"
        ], [
          egg_cv_var_ccpipe="no"
        ])
        CXX="$ac_old_CXX"
    ])

    if test "$egg_cv_var_ccpipe" = "yes"; then
      CXX="$CXX -pipe"
    fi
  fi
])
