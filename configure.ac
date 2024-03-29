dnl Process this file with autoconf to produce a configure script.
dnl
dnl

AC_PREREQ([2.71])
AC_INIT([bdlib],[1.0.0],[bryan@shatow.net],[bdlib])
AC_CONFIG_SRCDIR(src/bdlib.h)
AM_SILENT_RULES([yes])
AC_CONFIG_MACRO_DIR([build/autotools/m4])
AC_CONFIG_AUX_DIR([build/autotools/aux])
AM_INIT_AUTOMAKE([1.11 subdir-objects foreign -Wall -Wno-portability serial-tests])
AM_MAINTAINER_MODE([disable])
#LT_INIT()
AC_CONFIG_HEADERS(src/config.h)
AC_COPYRIGHT([Copyright (c) 2006-2014, Bryan Drewery <bryan@shatow.net>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTIRBUTORS ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
])
AC_REVISION($Revision$)

AC_ARG_ENABLE([debug],
  AS_HELP_STRING(
    [--enable-debug],
    [enable maintainer debugging]
  ),
  USE_DEBUG="$enableval",
  USE_DEBUG="no"
)

# Setup build environment
dnl Search for newer gcc versions before older
# Don't let AC_PROG_CXX set -O2 in user CXXFLAGS when we want DEBUG.
if [[ "${USE_DEBUG}" = "yes" ]]; then
  : ${CXXFLAGS:=-g}
fi
AC_LANG([C++])
AC_PROG_CXX([clang++ g++ c++ CC])
dnl AC_PROG_CC([gcc34 gcc-3.4 gcc33 gcc3 gcc-3.3 gcc cc c1])
AC_REQUIRE_CPP
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])
AX_CXX_COMPILE_STDCXX_14([noext], [optional])

# Check for compiler features
CXX_FLAG_CHECK([AM_CXXFLAGS], [-fcolor-diagnostics], [colordiagnostics])
#CXX_FLAG_CHECK([AM_CXXFLAGS], [-Qunused-arguments], [unusedarguments])
CXX_FLAG_CHECK([AM_CXXFLAGS], [-fstack-protector-all], [stackprotector])

# Debug flags
if [[ "${USE_DEBUG}" = "yes" ]]; then
  DEBUG_CXXFLAGS="\
    -W -Wno-unused-parameter -Wdisabled-optimization \
    -Wmissing-format-attribute -fstrict-aliasing \
    -Weffc++ -Woverloaded-virtual -fno-omit-frame-pointer \
    -fno-inline \
    -g \
  "
#  CXX_FLAG_CHECK_LINK([DEBUG_CXXFLAGS], [-fsanitize=address], [fsanitize_address])
#  CXX_FLAG_CHECK_LINK([DEBUG_CXXFLAGS], [-coverage], [coverage])
  CXX_FLAG_CHECK([DEBUG_CXXFLAGS], [-Winvalid-pch], [invalidpch])
  CXX_FLAG_CHECK([DEBUG_CXXFLAGS], [-Wnoexcept], [wnoexcept])
  CXX_FLAG_CHECK([DEBUG_CXXFLAGS], [-Wdocumentation], [wdocumentation])
  if [[ "${ax_cv_prog_cc_wdocumentation}" = "yes" ]]; then
    CPPUNIT_CXXFLAGS="${CPPUNIT_CXXFLAGS} -Wno-documentation"
  fi
  CXX_FLAG_CHECK([DEBUG_CXXFLAGS], [-Wsuggest-attribute=pure -Wsuggest-attribute=const], [wsuggestattribute])
  if [[ "${ax_cv_prog_cc_wsuggestattribute}" = "yes" ]]; then
    CPPUNIT_CXXFLAGS="${CPPUNIT_CXXFLAGS} -Wno-suggest-attribute=pure -Wno-suggest-attribute=const"
  fi
  CXX_FLAG_CHECK([DEBUG_CXXFLAGS], [-Og], [Og])
  if [[ "${ax_cv_prog_cc_Og}" = "no" ]]; then
    DEBUG_CXXFLAGS="${DEBUG_CXXFLAGS} -O0"
  fi
else
  AM_CXXFLAGS="$AM_CXXFLAGS -DNDEBUG"
fi
AC_SUBST(AM_CXXFLAGS)
AC_SUBST(CPPUNIT_CXXFLAGS)
AC_SUBST(DEBUG_CXXFLAGS)

AC_ARG_WITH([cppunit], AS_HELP_STRING([--with-cppunit], [Build with the cppunit support]))
if test "x$with_cppunit" = "xyes"; then
  PKG_CHECK_MODULES([CPPUNIT], [cppunit >= 1.14.0])
fi

m4_warn([obsolete],
[The preprocessor macro `STDC_HEADERS' is obsolete.
  Except in unusual embedded environments, you can safely include all
  ISO C90 headers unconditionally.])dnl
# Autoupdate added the next two lines to ensure that your configure
# script's behavior did not change.  They are probably safe to remove.
AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP


# Checks for programs
AC_PROG_MAKE_SET
AC_PROG_AWK
AC_PROG_GREP
AC_PROG_SED
AC_PROG_YACC
AC_PROG_RANLIB
AC_CHECK_PROG(BASENAME, basename, basename)
AC_CHECK_PROG(STRIP, strip, strip, touch)
AC_CHECK_PROG(DIFF, gdiff, gdiff, diff)
AC_CHECK_PROG(UNAME, uname, uname)
AC_CHECK_PROG(CCACHE, ccache, ccache)
AC_CHECK_PROG(DISTCC, distcc, distcc)
EGG_PROG_HEAD_1

# Checks for header files
AC_CHECK_HEADERS([stdarg.h fcntl.h netinet/in.h sys/socket.h])

#AC_C_CONST
AC_C_VOLATILE
AC_C_BIGENDIAN(AC_DEFINE(B_ENDIAN, 1, [big endian]),AC_DEFINE(L_ENDIAN, 1, [little endian]))
#AC_C_INLINE
AC_C_CHAR_UNSIGNED
#AC_C_STRINGIZE
#AC_C_PROTOTYPES

AC_FUNC_MMAP

AC_CHECK_FUNCS([memmem select])

SCRIPT_TCL

AH_BOTTOM([#include "config-sanity.h"])

AC_CONFIG_FILES([
	Makefile 
])
AC_OUTPUT
