dnl Process this file with autoconf to produce a configure script.
dnl
dnl $Id$
dnl

AC_PREREQ(2.59)
AC_INIT([Wraith Botpack], [2.0.0], [wraith@shatow.net], [wraith])
AC_CONFIG_SRCDIR(src/main.h)
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
AC_PROG_CXX([g++41 g++-4.1 g++40 g++-4.0 g++4 g++-3.3 g++33 g++-3 g++3 g++ c++ gpp aCC CC cxx cc++ cl FCC KCC RCC xlC_r xlC])
dnl AC_PROG_CC([gcc34 gcc-3.4 gcc33 gcc3 gcc-3.3 gcc cc c1])
AC_REQUIRE_CPP

# Check for compiler features
EGG_CHECK_CCWALL
EGG_CHECK_CCPIPE
EGG_CHECK_CCSTATIC

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
AC_CHECK_PROG(OBJCOPY, objcopy, [objcopy --remove-section=.note --remove-section=.comment], touch)
AC_CHECK_PROG(VALGRIND, valgrind, [valgrind --tool=memcheck --leak-check=yes --db-attach=no --show-reachable=yes --leak-resolution=high -q])
AC_CHECK_PROG(STRIP, strip, strip, touch)
AC_CHECK_PROG(DIFF, gdiff, gdiff, diff)
AC_CHECK_PROG(UNAME, uname, uname)
AC_CHECK_PROG(CCACHE, ccache, ccache)
AC_CHECK_PROG(DISTCC, distcc, distcc)

# Test the os and set the module linking settings
EGG_CHECK_OS

# Check for IPv6 support
#EGG_IPV6_SUPPORTED
EGG_IPV6_OPTIONS
EGG_CHECK_SOCKLEN_T

# Check for inconsistent random() functions
EGG_CHECK_RANDOM_MAX

# Create static binaries
AC_SUBST(CXXDEBUG)dnl

#checkpoint
AC_CACHE_SAVE 

# Checks for system libraries
EGG_CHECK_LIBS

# Checks for header files
AC_HEADER_DIRENT
#checkpoint
AC_CACHE_SAVE 
AC_CHECK_HEADERS([stdarg.h arpa/inet.h fcntl.h limits.h locale.h netinet/in.h])
AC_CHECK_HEADERS([sys/file.h sys/ioctl.h sys/param.h sys/socket.h sys/ptrace.h paths.h])

#checkpoint
AC_CACHE_SAVE
#AC_CHECK_HEADERS(openssl/ssl.h openssl/crypto.h)
#AC_CHECK_HEADERS(zlib.h)
#EGG_CHECK_ZLIB
#EGG_CHECK_SSL


AC_SUBST(ZLIB)dnl
#AC_SUBST(SSL)dnl

# Checks for typedefs, structures, and compiler characteristics
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UID_T
AC_TYPE_LONG_LONG_INT
AC_TYPE_INT8_T
AC_TYPE_UINT8_T
AC_TYPE_INT16_T
AC_TYPE_UINT16_T
AC_TYPE_INT32_T
AC_TYPE_UINT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT64_T
AC_CHECK_SIZEOF(short, 0)
AC_CHECK_SIZEOF(int, 0)
AC_CHECK_SIZEOF(long, 0)
AC_CHECK_SIZEOF(long long,0)


#AC_C_CONST
AC_C_VOLATILE
AC_C_BIGENDIAN(AC_DEFINE(B_ENDIAN, 1, [big endian]),AC_DEFINE(L_ENDIAN, 1, [little endian]))
#AC_C_INLINE
AC_C_CHAR_UNSIGNED
#AC_C_STRINGIZE
#AC_C_PROTOTYPES

#checkpoint
AC_CACHE_SAVE 


AC_STRUCT_TIMEZONE

# Checks for library functions
AC_FUNC_FORK
AC_FUNC_CHOWN
AC_FUNC_MKTIME
AC_FUNC_MMAP


AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
EGG_FUNC_TIMESPEC

#checkpoint
AC_CACHE_SAVE
AC_CHECK_FUNCS([fsync getrusage isascii])
AC_CHECK_FUNCS([random srandom gettime])
AC_CHECK_FUNCS([strcasecmp strncasecmp])

#checkpoint
AC_CACHE_SAVE

EGG_CHECK_FUNC_UNAME

#FIND_WRAPS
DO_DEPS

AC_CONFIG_FILES([
	Makefile 
	src/Makefile 
	lib/bdlib/Makefile 
	lib/bdlib/tests/Makefile
	lib/wraith/Makefile
	lib/wraith/tests/Makefile
	lib/openssl/Makefile
	build/Makefile
	build/buildinfo.h
	src/tests/Makefile
])
AC_OUTPUT

AH_TEMPLATE(HAVE_ST_MTIM, [Define to 1 if your struct stat has an st_mtim member])
AH_TEMPLATE(HAVE_ST_MTIMESPEC, [Define to 1 if your struct stat has an st_mtimespec member])
AH_TEMPLATE(HAVE_TIMESPEC, [Define to 1 if you have struct timespec in sys/time.h])
AH_TEMPLATE(HAVE_TIMESPECSUB2, [Define to 1 if you have a timespecsub macro or function that takes two arguments (not three)])
AH_BOTTOM([/*
 * Macros to pull sec and nsec parts of mtime from struct stat.
 */
#ifdef HAVE_ST_MTIM
# define mtim_getsec(_x)        ((_x).st_mtim.tv_sec)
# define mtim_getnsec(_x)       ((_x).st_mtim.tv_nsec)
#else
# ifdef HAVE_ST_MTIMESPEC
#  define mtim_getsec(_x)       ((_x).st_mtimespec.tv_sec)
#  define mtim_getnsec(_x)      ((_x).st_mtimespec.tv_nsec)
# else
#  define mtim_getsec(_x)       ((_x).st_mtime)
#  define mtim_getnsec(_x)      (0)
# endif /* HAVE_ST_MTIMESPEC */
#endif /* HAVE_ST_MTIM */

/*
 * Enable IPv6 debugging?
 */
#define DEBUG_IPV6 1
#define HAVE_IPV6 1

/* IPv6 sanity checks. */
#ifdef USE_IPV6
#  ifndef HAVE_IPV6
#    undef USE_IPV6
#  endif
#  ifndef HAVE_GETHOSTBYNAME2
#    ifndef HAVE_GETIPNODEBYNAME
#      undef USE_IPV6
#    endif
#  endif
#endif
])

cd lib/openssl && ./configure
