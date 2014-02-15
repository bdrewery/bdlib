dnl Process this file with autoconf to produce a configure script.
dnl
dnl

AC_PREREQ(2.59)
AC_INIT([bdlib], [1.0.0], [bryan@shatow.net], [bdlib])
AC_CONFIG_SRCDIR(src/bdlib.h)
AM_SILENT_RULES([yes])
AC_CONFIG_MACRO_DIR([build/autotools/m4])
AC_CONFIG_AUX_DIR([build/autotools/aux])
AM_INIT_AUTOMAKE([1.11 subdir-objects foreign -Wall -Wno-portability])
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

THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
])
AC_REVISION($Revision$)

# Setup build environment
dnl Search for newer gcc versions before older
AC_LANG([C++])
AC_PROG_CXX([clang++ g++49 g++-4.9 g++48 g++-4.8 g++47 g++-4.7 g++46 g++-4.6 g++ c++ gpp aCC CC cxx cc++ cl FCC KCC RCC xlC_r xlC])
dnl AC_PROG_CC([gcc34 gcc-3.4 gcc33 gcc3 gcc-3.3 gcc cc c1])
AC_REQUIRE_CPP
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])

# Check for compiler features
EGG_CHECK_CCPIPE
EGG_CHECK_CCSTATIC
CXX_FLAG_CHECK([AM_CXXFLAGS], [-fcolor-diagnostics], [colordiagnostics])
CXX_FLAG_CHECK([AM_CXXFLAGS], [-Qunused-arguments], [unusedarguments])
CXX_FLAG_CHECK([AM_CXXFLAGS], [-fstack-protector-all], [stackprotector])
CXX_FLAG_CHECK([AM_CXXFLAGS], [-Winvalid-pch], [invalidpch])
CXX_FLAG_CHECK([WDOCUMENTATION], [-Wdocumentation], [wdocumentation])
AC_SUBST(AM_CXXFLAGS)
AC_SUBST(WDOCUMENTATION)

AM_PATH_CPPUNIT(1.9.10)

AC_HEADER_STDC

#checkpoint
AC_CACHE_SAVE 

# Checks for programs
AC_PROG_MAKE_SET
AC_PROG_AWK
AC_PROG_GREP
AC_PROG_SED
AC_PROG_YACC
AC_PROG_RANLIB
AC_CHECK_PROG(BASENAME, basename, basename)
AC_CHECK_PROG(VALGRIND, valgrind, [valgrind --tool=memcheck --leak-check=yes --db-attach=no --show-reachable=yes --leak-resolution=high --track-fds=yes --track-origins=yes --undef-value-errors=yes -q])
AC_CHECK_PROG(STRIP, strip, strip, touch)
AC_CHECK_PROG(DIFF, gdiff, gdiff, diff)
AC_CHECK_PROG(UNAME, uname, uname)
AC_CHECK_PROG(CCACHE, ccache, ccache)
AC_CHECK_PROG(DISTCC, distcc, distcc)
EGG_PROG_HEAD_1

#checkpoint
AC_CACHE_SAVE 

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

AC_CHECK_FUNCS([select])

#checkpoint
AC_CACHE_SAVE

SCRIPT_TCL

AC_CACHE_SAVE

#FIND_WRAPS

AH_BOTTOM([#include "config-sanity.h"])

AC_CONFIG_FILES([
	Makefile 
])
AC_OUTPUT
