dnl Process this file with autoconf to produce a configure script.
dnl
dnl

AC_PREREQ(2.59)
AC_INIT([bdlib], [1.0.0], [bryan@shatow.net], [bdlib])
AC_CONFIG_SRCDIR(src/bdlib.h)
AC_CONFIG_HEADERS(build/config.h)
AC_COPYRIGHT([Copyright (C) Bryan Drewery
This program is private and may not be distributed, modified
or used without express permission of Bryan Drewery.

THIS PROGRAM IS DISTRIBUTED WITHOUT ANY WARRANTY.
IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY
FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
DERIVATIVES THEREOF, EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE
IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE
NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
MODIFICATIONS.
])
AC_REVISION($Revision$)

# Setup build environment
dnl Search for newer gcc versions before older
AC_LANG(C++)
AC_PROG_CXX([g++49 g++-4.9 g++48 g++-4.8 g++47 g++-4.7 g++46 g++-4.6 g++45 g++-4.5 g++44 g++-4.4 g++41 g++-4.1 g++40 g++-4.0 g++4 g++-3.3 g++33 g++-3 g++3 g++ c++ gpp aCC CC cxx cc++ cl FCC KCC RCC xlC_r xlC])
dnl AC_PROG_CC([gcc34 gcc-3.4 gcc33 gcc3 gcc-3.3 gcc cc c1])
AC_REQUIRE_CPP

# Check for compiler features
EGG_CHECK_CCWALL
EGG_CHECK_CCPIPE
CHECK_CC_COLOR_DIAGNOSTICS
EGG_CHECK_CCSTATIC
CXX_FLAG_CHECK([DEBCXXFLAGS], [-fstack-protector-all], [stackprotector])

AM_PATH_CPPUNIT(1.9.10)

AC_HEADER_STDC

#checkpoint
AC_CACHE_SAVE 

CHECK_DEPMODE

# Checks for programs
AC_PROG_MAKE_SET
AC_PROG_AWK
AC_PROG_GREP
AC_PROG_SED
AC_PROG_YACC
AC_CHECK_PROG(BASENAME, basename, basename)
AC_CHECK_PROG(VALGRIND, valgrind, [valgrind --tool=memcheck --leak-check=yes --db-attach=no --show-reachable=yes --leak-resolution=high --track-fds=yes --track-origins=yes --undef-value-errors=yes -q])
AC_CHECK_PROG(STRIP, strip, strip, touch)
AC_CHECK_PROG(DIFF, gdiff, gdiff, diff)
AC_CHECK_PROG(UNAME, uname, uname)
AC_CHECK_PROG(CCACHE, ccache, ccache)
AC_CHECK_PROG(DISTCC, distcc, distcc)

AC_SUBST(CXXDEBUG)dnl
AC_SUBST(DEBCXXFLAGS)

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

#FIND_WRAPS
DO_DEPS

AC_CONFIG_FILES([
	Makefile 
	src/Makefile 
	build/Makefile
	src/tests/Makefile
])
AC_OUTPUT
