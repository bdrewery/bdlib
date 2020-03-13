#ifndef _STDIO_GETS_HACK
#define _STDIO_GETS_HACK 1

/*
 * Hack for GCC 4 on FreeBSD without gets; wrong gcc config.
 *     /usr/local/lib/gcc49/include/c++/cstdio:124:11: error: '::gets' has not been declared
 */
#if __cplusplus <= 201103L && defined(__GNUC__)
#undef gets
extern "C" char* gets (char* __s) __attribute__((__deprecated__));
#define _GLIBCXX_HAVE_GETS 1
#endif

#endif	/* !_STDIO_GETS_HACK */
