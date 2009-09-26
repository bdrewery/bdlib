
// TR1 functional header -*- C++ -*-

// Copyright (C) 2004, 2005 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
// USA.

// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.

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
