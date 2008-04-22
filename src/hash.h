/* $Id$ */
/* FROM TR1 */

#ifndef _BD_HASH_H
#define _BD_HASH_H 1

#include "bdlib.h"
#include <sys/types.h>

BDLIB_NS_BEGIN

template<typename T>
  struct hash;

#define tr1_hashtable_define_trivial_hash(T)                                      \
template<>                                                                        \
  struct hash<T>                                                                  \
  {                                                                               \
    inline size_t operator()(T val) const { return static_cast<size_t>(val); }    \
  }

tr1_hashtable_define_trivial_hash(bool);
tr1_hashtable_define_trivial_hash(char);
tr1_hashtable_define_trivial_hash(signed char);
tr1_hashtable_define_trivial_hash(unsigned char);
//tr1_hashtable_define_trivial_hash(wchar_t);
tr1_hashtable_define_trivial_hash(short);
tr1_hashtable_define_trivial_hash(int);
tr1_hashtable_define_trivial_hash(long);
tr1_hashtable_define_trivial_hash(unsigned short);
tr1_hashtable_define_trivial_hash(unsigned int);
tr1_hashtable_define_trivial_hash(unsigned long);

#undef tr1_hashtable_define_trivial_hash

/** TODO:
float
double
long double
char *
const char *
String
*/

template<typename T>
  struct hash<T*>
  {
    inline size_t operator()(T* p) const { return reinterpret_cast<size_t>(p); }
  };

BDLIB_NS_END
#endif
