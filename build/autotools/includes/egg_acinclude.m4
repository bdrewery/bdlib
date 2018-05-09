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

dnl EGG_CHECK_CCWALL()
dnl
dnl See if the compiler supports -Wall.
dnl
AC_DEFUN([EGG_CHECK_CCWALL],
[
  if test -n "$GXX" && test -z "$no_wall"; then
    AC_CACHE_CHECK([whether the compiler understands -Wall], egg_cv_var_ccwall, [
      ac_old_CXXFLAGS="$CXXFLAGS"
      CXXFLAGS="$CXXFLAGS -Wall"
       AC_COMPILE_IFELSE([AC_LANG_SOURCE([
         int main ()
         {
           return(0);
         }
       ])], [
         egg_cv_var_ccwall="yes"
       ], [
         egg_cv_var_ccwall="no"
       ])
      CXXFLAGS="$ac_old_CXXFLAGS"
    ])

    if test "$egg_cv_var_ccwall" = "yes"; then
      CXXFLAGS="$CXXFLAGS -Wall"
    fi
  fi
])

dnl @synopsis CXX_FLAGS_CHECK [var] [compiler flags] [cache name] [required]
dnl @summary check whether compiler supports given C++ flags or not
AC_DEFUN([CXX_FLAG_CHECK],
[
  AC_CACHE_CHECK([whether the compiler understands $2], egg_cv_prog_cc_$3, [
    AC_LANG_PUSH([C++])
    ac_saved_flags="$CXXFLAGS"
    CXXFLAGS="-Werror $2"
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])],
      [egg_cv_prog_cc_$3="yes"],
      [egg_cv_prog_cc_$3="no"],
    )
    CXXFLAGS="$ac_saved_flags"
    AC_LANG_POP([C++])
  ])

  if [[ "$egg_cv_prog_cc_$3" = "yes" ]]; then
    $1="$$1 $2"
  elif [[ -n "$4" ]]; then
      cat << 'EOF' >&2
configure: error:

  Your OS or C++ compiler does not support $2.
  This compile flag is required.

EOF
    exit 1
  fi
])

dnl @synopsis CXX_FLAG_CHECK_NO [var] [compiler flags] [no flag] [cache name] [required]
dnl @summary check whether compiler supports given C++ flags or not
AC_DEFUN([CXX_FLAG_CHECK_NO],
[
  AC_CACHE_CHECK([whether the compiler understands $3], ax_cv_prog_cc_$4, [
    AC_LANG_PUSH([C++])
    ac_saved_flags="$CXXFLAGS"
    CXXFLAGS="-Werror $2"
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])],
      [ax_cv_prog_cc_$4="yes"],
      [ax_cv_prog_cc_$4="no"],
    )
    CXXFLAGS="$ac_saved_flags"
    AC_LANG_POP([C++])
  ])

  if [[ "$ax_cv_prog_cc_$4" = "yes" ]]; then
    $1="$$1 $3"
  elif [[ -n "$5" ]]; then
      cat << 'EOF' >&2
configure: error:

  Your OS or C++ compiler does not support $3.
  This compile flag is required.

EOF
    exit 1
  fi
])


