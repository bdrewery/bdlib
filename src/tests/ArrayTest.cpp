/*
 * Copyright (c) 2006-2014, Bryan Drewery <bryan@shatow.net>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTIRBUTORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* ArrayTest.c
 *
 */
#include "ArrayTest.h"
#include "HashTable.h"
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <type_traits>
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION (ArrayTest);

void ArrayTest :: setUp (void)
{
    // set up test environment (initializing objects)
    str_a = new Array<String>();
    str_b = new Array<String>(20);
    int_a = new Array<int>();
    int_b = new Array<int>();
    int_c = new Array<int>();
}

void ArrayTest :: tearDown (void)
{
    // finally delete objects
    delete str_a; delete str_b; delete str_c;
    delete char_a; delete char_b;
    delete int_a; delete int_b; delete int_c; delete int_d;
}

void ArrayTest :: sizeTest (void)
{
  CPPUNIT_ASSERT_EQUAL(size_t(0), str_a->size());
  CPPUNIT_ASSERT_EQUAL(size_t(0), str_b->size());
  CPPUNIT_ASSERT_EQUAL(true, str_b->isEmpty());
  CPPUNIT_ASSERT(str_b->capacity() >= 20);

//  CPPUNIT_ASSERT_EQUAL(size_t(2), str_c->size());
//  CPPUNIT_ASSERT(str_c->capacity() >= 2);
}

void ArrayTest :: push_popTest (void)
{
  str_a->push_back(String("Test1"));
  CPPUNIT_ASSERT_EQUAL(size_t(1), str_a->size());
  (*str_a) << String("Test2");
  CPPUNIT_ASSERT_EQUAL(size_t(2), str_a->size());
  CPPUNIT_ASSERT(str_a->capacity() >= 2);
  CPPUNIT_ASSERT_STRING_EQUAL("Test2", str_a->pop());
  CPPUNIT_ASSERT_STRING_EQUAL("Test1", str_a->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), str_a->size());
  CPPUNIT_ASSERT(str_a->capacity() >= 2);

  String foo("Test2");
  (*str_a) << foo;
  CPPUNIT_ASSERT_STRING_EQUAL("Test2", str_a->pop());
  CPPUNIT_ASSERT_STRING_EQUAL("Test2", foo);
  CPPUNIT_ASSERT_EQUAL(size_t(0), str_a->size());
  (*str_a) << std::move(foo);
  CPPUNIT_ASSERT_STRING_EQUAL("Test2", str_a->pop());
  CPPUNIT_ASSERT_STRING_EQUAL("", foo);
  CPPUNIT_ASSERT_EQUAL(size_t(0), str_a->size());

  String a = "Testa";
  String b = "Testb";
  String c = b;
  str_a->push_back(a);

  str_a->push_back(a);
  str_a->pop();

  str_a->push_back(b);
  str_a->push_back(a);
  str_a->push_back(b);
  str_a->push_back(b);
  CPPUNIT_ASSERT_EQUAL(size_t(5), str_a->size());

  String res = str_a->pop();
  CPPUNIT_ASSERT_STRING_EQUAL(b, res);
  CPPUNIT_ASSERT_EQUAL(size_t(4), str_a->size());

  b += "changed";
  (*str_a) >> res;
  CPPUNIT_ASSERT_EQUAL(size_t(3), str_a->size());
  CPPUNIT_ASSERT_STRING_EQUAL(c, res);//b
  b -= 7;

  CPPUNIT_ASSERT_STRING_EQUAL(a, str_a->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(2), str_a->size());

  CPPUNIT_ASSERT_STRING_EQUAL(b, str_a->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(1), str_a->size());

  CPPUNIT_ASSERT_STRING_EQUAL(a, str_a->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), str_a->size());

  //CPPUNIT_ASSERT_STRING_EQUAL(String(), str_a->pop());

  (*str_a) << "1";
  (*str_a) << "2";
  (*str_a) << "3";
  (*str_a) << "4";
  CPPUNIT_ASSERT_STRING_EQUAL("1 2 3 4", str_a->join(' '));

  String hm;
  hm = (*str_a).pop();
  CPPUNIT_ASSERT_STRING_EQUAL("1 2 3", str_a->join(' '));
  CPPUNIT_ASSERT_STRING_EQUAL("4", hm);
  CPPUNIT_ASSERT_EQUAL(size_t(3), str_a->size());

  hm = (*str_a).shift();
  CPPUNIT_ASSERT_STRING_EQUAL("2 3", str_a->join(' '));
  CPPUNIT_ASSERT_STRING_EQUAL("1", hm);
  CPPUNIT_ASSERT_EQUAL(size_t(2), str_a->size());

  hm = Array<String>{"Test0", "Test1"}.pop();
  CPPUNIT_ASSERT_STRING_EQUAL("Test1", hm);

  hm = Array<String>{"Test0", "Test1"}.shift();
  CPPUNIT_ASSERT_STRING_EQUAL("Test0", hm);

  (*str_a) = Array<String>();
  (*str_a) << "Test";
  (*str_a) << "1 2 3";
  (*str_a) << "Last 1";
  CPPUNIT_ASSERT_STRING_EQUAL("\"Test\" \"1 2 3\" \"Last 1\"", str_a->join(" ", true));
}

void ArrayTest :: arrayConsTest(void)
{
  String carray2[] = {"2Test1", "2Test2", "2Test3"};
  str_c = new Array<String>(carray2, 2);
  CPPUNIT_ASSERT_EQUAL(size_t(2), str_c->size());
  CPPUNIT_ASSERT_STRING_EQUAL("2Test2", str_c->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(1), str_c->size());
  CPPUNIT_ASSERT_STRING_EQUAL("2Test1", str_c->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), str_c->size());

  int carray[] = {1, 2, 0};
  int_d = new Array<int>(carray, sizeof(carray) / sizeof(carray[0]));
  CPPUNIT_ASSERT_EQUAL(size_t(3), int_d->size());
  CPPUNIT_ASSERT_EQUAL(0, int_d->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(2), int_d->size());
  CPPUNIT_ASSERT_EQUAL(2, int_d->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(1), int_d->size());
  CPPUNIT_ASSERT_EQUAL(1, int_d->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), int_d->size());
}

void ArrayTest :: clearTest (void)
{
  for (int i = 0; i < 10; ++i)
    int_a->push_back(i);
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_a->size());
  int_a->clear();
  CPPUNIT_ASSERT_EQUAL(size_t(0), int_a->size());
  CPPUNIT_ASSERT_EQUAL(true, int_a->isEmpty());
  CPPUNIT_ASSERT_THROW(int_a->at(0), std::out_of_range);
}


void ArrayTest :: refTest (void)
{
  for (int i = 0; i < 10; ++i)
    int_a->push_back(i);
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_a->size());
  CPPUNIT_ASSERT_EQUAL(size_t(1), int_a->rcount());
  *int_b = *int_a;
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_a->size());
  CPPUNIT_ASSERT_EQUAL(size_t(2), int_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_b->size());
  CPPUNIT_ASSERT_EQUAL(size_t(2), int_b->rcount());

  int_d = new Array<int>();
  *int_d = *int_a;
  *int_c = *int_a;
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_a->size());
  CPPUNIT_ASSERT_EQUAL(size_t(4), int_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_b->size());
  CPPUNIT_ASSERT_EQUAL(size_t(4), int_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_c->size());
  CPPUNIT_ASSERT_EQUAL(size_t(4), int_c->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_d->size());
  CPPUNIT_ASSERT_EQUAL(size_t(4), int_d->rcount());

  int_b->push_back(99);
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_a->size());
  CPPUNIT_ASSERT_EQUAL(size_t(3), int_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(11), int_b->size());
  CPPUNIT_ASSERT_EQUAL(size_t(1), int_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_c->size());
  CPPUNIT_ASSERT_EQUAL(size_t(3), int_c->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_d->size());
  CPPUNIT_ASSERT_EQUAL(size_t(3), int_d->rcount());

  for (int i = 9; i >= 0; --i)
    CPPUNIT_ASSERT_EQUAL(i, int_a->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), int_a->size());
  CPPUNIT_ASSERT_EQUAL(size_t(3), int_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(11), int_b->size());
  CPPUNIT_ASSERT_EQUAL(size_t(1), int_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_c->size());
  CPPUNIT_ASSERT_EQUAL(size_t(3), int_c->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_d->size());
  CPPUNIT_ASSERT_EQUAL(size_t(3), int_d->rcount());

  CPPUNIT_ASSERT_EQUAL(99, int_b->pop());
  for (int i = 9; i > 0; --i)
    CPPUNIT_ASSERT_EQUAL(i, int_b->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), int_a->size());
  CPPUNIT_ASSERT_EQUAL(size_t(3), int_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), int_b->size());
  CPPUNIT_ASSERT_EQUAL(size_t(1), int_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_c->size());
  CPPUNIT_ASSERT_EQUAL(size_t(3), int_c->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_d->size());
  CPPUNIT_ASSERT_EQUAL(size_t(3), int_d->rcount());

  *int_b = *int_c;
  CPPUNIT_ASSERT_EQUAL(size_t(0), int_a->size());
  CPPUNIT_ASSERT_EQUAL(size_t(4), int_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_b->size());
  CPPUNIT_ASSERT_EQUAL(size_t(4), int_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_c->size());
  CPPUNIT_ASSERT_EQUAL(size_t(4), int_c->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_d->size());
  CPPUNIT_ASSERT_EQUAL(size_t(4), int_d->rcount());
}

void ArrayTest :: joinTest(void)
{
  str_a->push_back("Test1");
  str_a->push_back("Test2");
  str_a->push_back("Test3");
  CPPUNIT_ASSERT_STRING_EQUAL("Test1 Test2 Test3", str_a->join(' '));
  CPPUNIT_ASSERT_EQUAL(size_t(3), str_a->size());
  CPPUNIT_ASSERT_STRING_EQUAL("Test3", str_a->pop());
  CPPUNIT_ASSERT_STRING_EQUAL("Test2", str_a->pop());
  CPPUNIT_ASSERT_STRING_EQUAL("Test1", str_a->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), str_a->size());
}

void ArrayTest :: compareTest(void)
{
  str_a->push_back("Test1");
  str_a->push_back("Test2");

  str_b->push_back("Test1");
  CPPUNIT_ASSERT_EQUAL(false, (*str_a) == (*str_b));
  str_b->push_back("Test2");

  CPPUNIT_ASSERT_EQUAL(true, (*str_a) == (*str_b));
}

void ArrayTest :: findTest(void)
{
  str_a->push_back("Test1");
  str_a->push_back("Test2");
  CPPUNIT_ASSERT_EQUAL(size_t(-1), str_a->find("Test3"));
  CPPUNIT_ASSERT_EQUAL(size_t(0), str_a->find("Test1"));
  CPPUNIT_ASSERT_EQUAL(size_t(1), str_a->find("Test2"));
}

void ArrayTest :: indexTest(void)
{
  str_c = new Array<String>(20);

  str_a->push_back("Test1");
  str_a->push_back("Test2");
  CPPUNIT_ASSERT_STRING_EQUAL("Test1", (*str_a)[0]);
  CPPUNIT_ASSERT_STRING_EQUAL("Test2", (*str_a)[1]);

  (*str_b) = (*str_a);
  (*str_c) = (*str_a);

  size_t ref_a, ref_b, ref_c, ref_str_b_1, ref_str_a_1, ref_str_a, ref_str_b, ref_str_c;

  ref_str_a = str_a->rcount();
  ref_str_b = str_b->rcount();
  ref_str_c = str_c->rcount();
  CPPUNIT_ASSERT_EQUAL(size_t(3), ref_str_a);
  CPPUNIT_ASSERT_EQUAL(size_t(3), ref_str_b);
  CPPUNIT_ASSERT_EQUAL(size_t(3), ref_str_c);
  CPPUNIT_ASSERT_EQUAL(ref_str_a, ref_str_b);

  CPPUNIT_ASSERT_EQUAL(str_a->rptr(), str_b->rptr());
  CPPUNIT_ASSERT_EQUAL(str_a->rptr(), str_c->rptr());
  CPPUNIT_ASSERT_EQUAL((*str_a)[1].rptr(), (*str_b)[1].rptr());
  CPPUNIT_ASSERT_EQUAL((*str_b)[1].rptr(), (*str_c)[1].rptr());
  String a("Test5");
  String b(a);
  CPPUNIT_ASSERT_EQUAL(a.rptr(), b.rptr());
  (*str_a)[1] = a;
  CPPUNIT_ASSERT_EQUAL(a.rptr(), b.rptr());
  CPPUNIT_ASSERT_EQUAL(a.rptr(), (*str_a)[1].rptr());
  CPPUNIT_ASSERT_EQUAL(b.rptr(), (*str_a)[1].rptr());
  CPPUNIT_ASSERT(a.rptr() != str_b[1].rptr());
  CPPUNIT_ASSERT(a.rptr() != str_c[1].rptr());
  CPPUNIT_ASSERT_EQUAL((*str_b)[1].rptr(), (*str_c)[1].rptr());
  CPPUNIT_ASSERT_EQUAL(str_b->rptr(), str_c->rptr());
  ref_str_a = 1;
  --ref_str_b;
  ref_str_c = ref_str_b;
  CPPUNIT_ASSERT_EQUAL(ref_str_a, str_a->rcount());
  CPPUNIT_ASSERT_EQUAL(ref_str_b, str_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), ref_str_a);
  CPPUNIT_ASSERT_EQUAL(size_t(2), ref_str_b);
  CPPUNIT_ASSERT_EQUAL(size_t(2), ref_str_c);
  CPPUNIT_ASSERT_EQUAL(ref_str_b, ref_str_c);
  ref_a = a.rcount();
  ref_str_a_1 = ref_a;
  ref_b = b.rcount();
  CPPUNIT_ASSERT_EQUAL(size_t(3), ref_a);
  CPPUNIT_ASSERT_EQUAL(size_t(3), ref_str_a_1);
  CPPUNIT_ASSERT_EQUAL(a.rptr(), (*str_a)[1].rptr());
  CPPUNIT_ASSERT_EQUAL(ref_a, ref_b);
  CPPUNIT_ASSERT_STRING_EQUAL("Test1", (*str_a)[0]);
  CPPUNIT_ASSERT_STRING_EQUAL(a, (*str_a)[1]);
  CPPUNIT_ASSERT_EQUAL(ref_str_a, str_a->rcount());
  CPPUNIT_ASSERT_EQUAL(ref_str_b, str_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), ref_str_a);
  CPPUNIT_ASSERT_EQUAL(size_t(2), ref_str_b);
  CPPUNIT_ASSERT_EQUAL(size_t(2), ref_str_c);
  CPPUNIT_ASSERT_EQUAL(ref_str_b, ref_str_c);
  CPPUNIT_ASSERT_STRING_EQUAL(a, b);
  CPPUNIT_ASSERT_EQUAL(ref_a, a.rcount());
  CPPUNIT_ASSERT_EQUAL(ref_a+1, static_cast<String>((*str_a)[1]).rcount());
  CPPUNIT_ASSERT_EQUAL(ref_a, (*str_a)[1].rcount());
  CPPUNIT_ASSERT_EQUAL(ref_str_a, str_a->rcount());
  CPPUNIT_ASSERT_EQUAL(ref_str_b, str_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), ref_str_a);
  CPPUNIT_ASSERT_EQUAL(size_t(2), ref_str_b);
  CPPUNIT_ASSERT_EQUAL(size_t(2), ref_str_c);
  CPPUNIT_ASSERT_EQUAL(ref_str_b, ref_str_c);
  CPPUNIT_ASSERT_EQUAL(ref_str_a_1+1, static_cast<String>((*str_a)[1]).rcount());
  CPPUNIT_ASSERT_EQUAL(ref_a, a.rcount());
  CPPUNIT_ASSERT_STRING_EQUAL("Test5", static_cast<String>((*str_a)[1]).c_str());
  CPPUNIT_ASSERT_EQUAL(ref_a, a.rcount());
  CPPUNIT_ASSERT_STRING_EQUAL("Test5", (*str_a)[1].get().c_str());
  CPPUNIT_ASSERT_STRING_EQUAL("Test5", static_cast<const Array<String>>((*str_a))[1].c_str());
  CPPUNIT_ASSERT_EQUAL(ref_str_a, str_a->rcount());
  CPPUNIT_ASSERT_EQUAL(ref_str_b, str_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), ref_str_a);
  CPPUNIT_ASSERT_EQUAL(size_t(2), ref_str_b);
  CPPUNIT_ASSERT_EQUAL(size_t(2), ref_str_c);
  CPPUNIT_ASSERT_EQUAL(ref_str_b, ref_str_c);
  CPPUNIT_ASSERT_EQUAL(ref_a, a.rcount());
  CPPUNIT_ASSERT_EQUAL(ref_a+1, static_cast<String>((*str_a)[1]).rcount());
  CPPUNIT_ASSERT_EQUAL(ref_str_a_1+1, static_cast<String>((*str_a)[1]).rcount());

  CPPUNIT_ASSERT_STRING_EQUAL("Test1", (*str_b)[0]);
  CPPUNIT_ASSERT_STRING_EQUAL("Test2", (*str_b)[1]);
  CPPUNIT_ASSERT_EQUAL(ref_str_a, str_a->rcount());
  CPPUNIT_ASSERT_EQUAL(ref_str_b, str_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), ref_str_a);
  CPPUNIT_ASSERT_EQUAL(size_t(2), ref_str_b);
  CPPUNIT_ASSERT_EQUAL(size_t(2), ref_str_c);
  CPPUNIT_ASSERT_EQUAL(ref_str_b, ref_str_c);
  CPPUNIT_ASSERT_EQUAL(size_t(1), (*str_b)[1].rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), (*str_b)[1].get().rcount());
  CPPUNIT_ASSERT_EQUAL((*str_b)[1].rcount(), (*str_b)[1].get().rcount());
  /* For some reason casting (which uses get()) causes a temporary ref  */
  CPPUNIT_ASSERT_EQUAL(size_t(1), (*str_b)[1].rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(2), static_cast<const String>((*str_b)[1]).rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(2), static_cast<String>((*str_b)[1]).rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(2), String((*str_b)[1]).rcount());
  {
    /* Clang and GCC differ in the expected rcount if .rcount() is called in the assertion. */
    const size_t rcount = (*str_b)[1].rcount();
    CPPUNIT_ASSERT_EQUAL(rcount+1, String((*str_b)[1]).rcount());
    CPPUNIT_ASSERT_EQUAL(rcount+1, static_cast<const String>((*str_b)[1]).rcount());
    CPPUNIT_ASSERT_EQUAL(rcount+1, static_cast<String>((*str_b)[1]).rcount());
  }
  ref_str_b_1 = (*str_b)[1].rcount();
  CPPUNIT_ASSERT_EQUAL(size_t(1), ref_str_b_1);
  CPPUNIT_ASSERT_EQUAL(size_t(1), (*str_b)[1].rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), (*str_b)[1].get().rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), (*str_b).at(1).rcount());
  CPPUNIT_ASSERT_EQUAL(ref_str_a, str_a->rcount());
  CPPUNIT_ASSERT_EQUAL(ref_str_b, str_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), ref_str_a);
  CPPUNIT_ASSERT_EQUAL(size_t(2), ref_str_b);
  CPPUNIT_ASSERT_EQUAL(size_t(2), ref_str_c);
  CPPUNIT_ASSERT_EQUAL(ref_str_b, ref_str_c);
  CPPUNIT_ASSERT_EQUAL(ref_str_b_1+1, static_cast<String>((*str_b)[1]).rcount());
  CPPUNIT_ASSERT_EQUAL(ref_a, a.rcount());
  /* Test non-const reference  */
  {
    size_t ref_tmp = 0, ref_tmp_str_b_1;
    ref_tmp_str_b_1 = ref_str_b_1;
    String d((*str_b)[1]);
    ++ref_tmp_str_b_1;
    ref_tmp = ref_tmp_str_b_1;
    CPPUNIT_ASSERT_STRING_EQUAL("Test2", d);
    CPPUNIT_ASSERT_EQUAL((*str_b).rptr(), (*str_c).rptr());
    CPPUNIT_ASSERT_EQUAL((*str_b)[1].rptr(), (*str_c)[1].rptr());
    CPPUNIT_ASSERT_EQUAL(d.rptr(), (*str_c)[1].rptr());
    /* 2 because str_b==str_c ref still so item[1] is the same ref */
    CPPUNIT_ASSERT_EQUAL(size_t(2), ref_tmp_str_b_1);
    CPPUNIT_ASSERT_EQUAL(ref_tmp_str_b_1+1, static_cast<String>((*str_b)[1]).rcount());
    CPPUNIT_ASSERT_EQUAL(size_t(2), ref_tmp);
    CPPUNIT_ASSERT_EQUAL(ref_tmp, ref_tmp_str_b_1);
    CPPUNIT_ASSERT_EQUAL(ref_tmp, d.rcount());
    CPPUNIT_ASSERT_EQUAL(ref_tmp+1, static_cast<String>((*str_b)[1]).rcount());
  }
  const String c((*str_b)[1]);
  CPPUNIT_ASSERT_STRING_EQUAL("Test2", (*str_b)[1]);
  CPPUNIT_ASSERT_STRING_EQUAL("Test2", (*str_c)[1]);
  CPPUNIT_ASSERT_STRING_EQUAL("Test2", c);
  CPPUNIT_ASSERT_EQUAL((*str_b).rptr(), (*str_c).rptr());
  CPPUNIT_ASSERT_EQUAL((*str_b)[1].rptr(), (*str_c)[1].rptr());
  CPPUNIT_ASSERT_EQUAL(c.rptr(), (*str_c)[1].rptr());
  ++ref_str_b_1;
  ref_c = ref_str_b_1;
  /* 2 because str_b==str_c ref still so item[1] is the same ref */
  CPPUNIT_ASSERT_EQUAL(size_t(2), ref_str_b_1);
  CPPUNIT_ASSERT_EQUAL(ref_str_b_1+1, static_cast<String>((*str_b)[1]).rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(2), ref_c);
  CPPUNIT_ASSERT_EQUAL(ref_c, ref_str_b_1);
  CPPUNIT_ASSERT_EQUAL(ref_c, c.rcount());
  CPPUNIT_ASSERT_EQUAL(ref_c+1, static_cast<String>((*str_b)[1]).rcount());
  (*str_b)[1] = a;
  /* str_b should split from str_c after being modified. */
  CPPUNIT_ASSERT((*str_b).rptr() != (*str_c).rptr());
  CPPUNIT_ASSERT_EQUAL((*str_b)[1].rptr(), a.rptr());
  CPPUNIT_ASSERT_EQUAL(c.rptr(), (*str_c)[1].rptr());
  CPPUNIT_ASSERT_STRING_EQUAL("Test2", c);
  /* ref_c is not modified since str_b split off; it was a ref to str_c so it has no impact on item-level refs. */
  CPPUNIT_ASSERT_EQUAL(size_t(2), ref_c);
  CPPUNIT_ASSERT_EQUAL(ref_c, c.rcount());
  ++ref_a;
  ++ref_b;
  CPPUNIT_ASSERT_EQUAL(size_t(4), ref_a);
  CPPUNIT_ASSERT_EQUAL(size_t(4), ref_b);
  CPPUNIT_ASSERT_EQUAL(ref_a, ref_b);
  CPPUNIT_ASSERT_EQUAL(ref_a, a.rcount());
  CPPUNIT_ASSERT_EQUAL(ref_b, b.rcount());
  CPPUNIT_ASSERT_EQUAL(a.rptr(), (*str_a)[1].rptr());
  CPPUNIT_ASSERT_EQUAL(a.rptr(), (*str_b)[1].rptr());
  CPPUNIT_ASSERT(a.rptr() != (*str_c)[1].rptr());
  ref_str_b = 1;
  --ref_str_c;
  CPPUNIT_ASSERT_EQUAL(ref_str_a, str_a->rcount());
  CPPUNIT_ASSERT_EQUAL(ref_str_b, str_b->rcount());
  CPPUNIT_ASSERT_EQUAL(ref_str_c, str_c->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), ref_str_a);
  CPPUNIT_ASSERT_EQUAL(size_t(1), ref_str_b);
  CPPUNIT_ASSERT_EQUAL(size_t(1), ref_str_c);
  CPPUNIT_ASSERT((*str_b)[1] != (*str_c)[1]);
  CPPUNIT_ASSERT_STRING_EQUAL("Test2", c);
  CPPUNIT_ASSERT_STRING_EQUAL("Test2", (*str_c)[1]);
  CPPUNIT_ASSERT_STRING_EQUAL("Test5", (*str_b)[1]);
  ref_str_b_1 = ref_c; /*not sure yet and sup with the later one?*/
  CPPUNIT_ASSERT_EQUAL(size_t(4), ref_b);
  CPPUNIT_ASSERT_EQUAL(ref_b, b.rcount());
  CPPUNIT_ASSERT_STRING_EQUAL("Test5", (*str_b)[1]);
  ++ref_str_a_1;
  ref_str_b_1 = ref_a;
  CPPUNIT_ASSERT_EQUAL(size_t(4), ref_str_b_1);
  CPPUNIT_ASSERT_EQUAL(ref_a, ref_str_b_1);
  CPPUNIT_ASSERT_EQUAL(ref_str_a_1+1, static_cast<String>((*str_a)[1]).rcount());
  CPPUNIT_ASSERT_EQUAL(ref_a, a.rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(4), ref_str_b_1);
  CPPUNIT_ASSERT_EQUAL(ref_str_b_1+1, static_cast<String>((*str_b)[1]).rcount());
  CPPUNIT_ASSERT_EQUAL(ref_a, a.rcount());
  CPPUNIT_ASSERT_STRING_EQUAL(a, (*str_b)[1]);
  CPPUNIT_ASSERT_EQUAL(ref_a, a.rcount());
  CPPUNIT_ASSERT_STRING_EQUAL(a, (*str_b)[1].get().c_str());
  CPPUNIT_ASSERT_EQUAL(ref_a, a.rcount());
  CPPUNIT_ASSERT_STRING_EQUAL(a, b);
  CPPUNIT_ASSERT_EQUAL(ref_a, a.rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(4), ref_str_a_1);
  CPPUNIT_ASSERT_EQUAL(ref_str_a_1+1, static_cast<String>((*str_a)[1]).rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(4), ref_str_b_1);
  CPPUNIT_ASSERT_EQUAL(ref_str_b_1+1, static_cast<String>((*str_b)[1]).rcount());
  CPPUNIT_ASSERT_EQUAL(ref_a, a.rcount());

  /* Check .get()= modifies the String not the Array */
  (*str_c) = (*str_b);
  ++ref_str_b;
  ref_str_c = ref_str_b;
  CPPUNIT_ASSERT_EQUAL(ref_str_a, str_a->rcount());
  CPPUNIT_ASSERT_EQUAL(ref_str_b, str_b->rcount());
  CPPUNIT_ASSERT_EQUAL(ref_str_c, str_c->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), ref_str_a);
  CPPUNIT_ASSERT_EQUAL(size_t(2), ref_str_b);
  CPPUNIT_ASSERT_EQUAL(size_t(2), ref_str_c);
  CPPUNIT_ASSERT_EQUAL((*str_b).rptr(), (*str_c).rptr());

  CPPUNIT_ASSERT_EQUAL(ref_a, a.rcount());
  CPPUNIT_ASSERT_EQUAL(ref_b, b.rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(4), ref_a);
  CPPUNIT_ASSERT_EQUAL(size_t(4), ref_b);
  CPPUNIT_ASSERT_EQUAL(size_t(4), ref_str_a_1);
  CPPUNIT_ASSERT_EQUAL(ref_str_a_1+1, static_cast<String>((*str_a)[1]).rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(4), ref_str_b_1);
  CPPUNIT_ASSERT_EQUAL(ref_str_b_1+1, static_cast<String>((*str_b)[1]).rcount());
  CPPUNIT_ASSERT_EQUAL((*str_b)[1].rptr(), a.rptr());
  CPPUNIT_ASSERT_EQUAL((*str_b)[1].rptr(), b.rptr());
  CPPUNIT_ASSERT_EQUAL((*str_b)[1].rptr(), (*str_a)[1].rptr());
  CPPUNIT_ASSERT_EQUAL((*str_b)[1].rptr(), (*str_c)[1].rptr());

  String e("StringE");
  size_t ref_e = 1;
  static_assert(
      !std::is_assignable<decltype((*str_b)[1].get()),
      decltype(e)>::value,
      "Array.Cref.get() should not be assignable");
  static_assert(
      !std::is_assignable<decltype((*str_b)[1].get()),
      const String>::value,
      "Array.Cref.get() should not be assignable");
  (*str_b)[1].mget() = e;
  /* ref_a unmodified since str_c holds the ref now */
  /* ref_str_a_1 unmodified since str_c holds the ref now */
  ++ref_e;
  ref_str_b_1 = ref_e;
  ref_str_b = 1;
  --ref_str_c;
  CPPUNIT_ASSERT_EQUAL(ref_str_a, str_a->rcount());
  CPPUNIT_ASSERT_EQUAL(ref_str_b, str_b->rcount());
  CPPUNIT_ASSERT_EQUAL(ref_str_c, str_c->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), ref_str_a);
  CPPUNIT_ASSERT_EQUAL(size_t(1), ref_str_b);
  CPPUNIT_ASSERT_EQUAL(size_t(1), ref_str_c);
  CPPUNIT_ASSERT((*str_b).rptr() != (*str_c).rptr());

  CPPUNIT_ASSERT_EQUAL(ref_a, a.rcount());
  CPPUNIT_ASSERT_EQUAL(ref_b, b.rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(4), ref_a);
  CPPUNIT_ASSERT_EQUAL(size_t(4), ref_b);
  CPPUNIT_ASSERT_EQUAL(size_t(4), ref_str_a_1);
  CPPUNIT_ASSERT_EQUAL(ref_str_a_1+1, static_cast<String>((*str_a)[1]).rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(2), ref_e);
  CPPUNIT_ASSERT_EQUAL(ref_e, e.rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(2), ref_str_b_1);
  CPPUNIT_ASSERT_EQUAL(ref_str_b_1+1, static_cast<String>((*str_b)[1]).rcount());
  CPPUNIT_ASSERT_EQUAL(ref_str_b_1, (*str_b)[1].rcount());
  CPPUNIT_ASSERT_EQUAL(e.rptr(), (*str_b)[1].rptr());
  CPPUNIT_ASSERT((*str_b)[1].rptr() != a.rptr());
  CPPUNIT_ASSERT_EQUAL((*str_b)[1].rptr(), e.rptr());
  CPPUNIT_ASSERT((*str_b)[1].rptr() != (*str_a)[1].rptr());
  CPPUNIT_ASSERT((*str_b)[1].rptr() != (*str_c)[1].rptr());
  CPPUNIT_ASSERT_EQUAL((*str_a)[1].rptr(), (*str_c)[1].rptr());

}

void ArrayTest :: subArrayTest (void)
{
  str_a->push_back("Test0");
  str_a->push_back("Test1");
  str_a->push_back("Test2");
  str_a->push_back("Test3");
  str_a->push_back("Test4");
  str_a->push_back("Test5");
  str_a->push_back("Test6");
  str_a->push_back("Test7");

  str_c = new Array<String>();
  (*str_c) = (*str_a);
  (*str_b) = (*str_a)(0, 2);

  CPPUNIT_ASSERT_ARRAY_EQUAL((*str_c), (*str_a));
  CPPUNIT_ASSERT_EQUAL(size_t(8), str_a->size());
  CPPUNIT_ASSERT_EQUAL(size_t(2), str_b->size());
  CPPUNIT_ASSERT_STRING_EQUAL("Test0", (*str_b)[0]);
  CPPUNIT_ASSERT_STRING_EQUAL("Test1", (*str_b)[1]);

  CPPUNIT_ASSERT_STRING_EQUAL("Test1", str_b->pop());
  CPPUNIT_ASSERT_STRING_EQUAL("Test0", (*str_b)[0]);
  CPPUNIT_ASSERT_EQUAL(size_t(1), str_b->size());

  // In place test
  Array<String> inplaceTest;
  inplaceTest.push_back("TEST0");
  inplaceTest.push_back("TEST1");
  inplaceTest.push_back("TEST2");
  inplaceTest.push_back("TEST3");
  (*str_b) = (*str_a);
  (*str_b)(0, 2) = inplaceTest;

  Array<String> expectedResults(*str_a);
  expectedResults[0] = "TEST0";
  expectedResults[1] = "TEST1";
  CPPUNIT_ASSERT_EQUAL(size_t(8), str_a->size());
  CPPUNIT_ASSERT_EQUAL(size_t(8), str_b->size());
  CPPUNIT_ASSERT_ARRAY_EQUAL((*str_b), expectedResults);
}

void ArrayTest :: resizeTest(void)
{
  String foo{"foo"};
  str_a->clear();
  str_a->resize(5, foo);
  CPPUNIT_ASSERT_EQUAL(size_t(5), (*str_a).length());
  CPPUNIT_ASSERT_STRING_EQUAL(foo, (*str_a)[0]);
  CPPUNIT_ASSERT_EQUAL(size_t(6), foo.rcount());
}

void ArrayTest :: hashTest(void)
{
  str_c = new Array<String>();

  (*str_a).push_back("test");
  (*str_a).push_back("test2");
  (*str_b).push_back("test");
  (*str_b).push_back("test2");
  (*str_c) = (*str_b);
  CPPUNIT_ASSERT_EQUAL(str_a->hash(), str_b->hash());
  CPPUNIT_ASSERT_EQUAL(str_a->hash(), str_c->hash());
  CPPUNIT_ASSERT_EQUAL(str_b->hash(), str_c->hash());

  str_a->clear();
  str_b->clear();
  (*str_a).push_back("test");
  (*str_a).push_back("test1");
  (*str_b).push_back("test");
  (*str_c) = (*str_b);
  CPPUNIT_ASSERT(str_a->hash() != str_b->hash());
  CPPUNIT_ASSERT(str_a->hash() != str_c->hash());
  CPPUNIT_ASSERT_EQUAL(str_b->hash(), str_c->hash());

  str_b->clear();
  (*str_b) = (*str_a)(0, 1);
  CPPUNIT_ASSERT(str_a->hash() != str_b->hash());
  (*str_b) = (*str_a)(0, 2);
  CPPUNIT_ASSERT(str_a->hash() == str_b->hash());

  *str_b = *str_a;

  HashTable< String, Array<String> > ht;
  ht["blaha"] = *str_a;

  HashTable< String, Array<String> > ht2;
  ht2["blahb"] = *str_b;

  CPPUNIT_ASSERT_ARRAY_EQUAL(ht["blaha"], ht2["blahb"]);

  ht2["blah3"] = *str_c;

  CPPUNIT_ASSERT(ht["blah"] != ht2["blah3"]);

  HashTable< Array<String>, int > ha1;
  HashTable< Array<String>, int > ha2;

  ha1[*str_b] = 1;
  ha2[*str_a] = 1;
  ha2[*str_c] = 5;
  CPPUNIT_ASSERT_EQUAL(ha1[*str_b], ha2[*str_b]);
  CPPUNIT_ASSERT(ha1[*str_b] != ha2[*str_c]);
}

void ArrayTest :: operatorsTest(void)
{
  str_a->push_back("1");
  str_a->push_back("2");
  str_b->push_back("3");
  str_b->push_back("4");

  str_c = new Array<String>();
  (*str_c) = (*str_a) + (*str_b);

  CPPUNIT_ASSERT_STRING_EQUAL("1 2 3 4", str_c->join(' '));
  CPPUNIT_ASSERT_EQUAL(size_t(4), str_c->size());

  (*str_c) += 1;
  CPPUNIT_ASSERT_STRING_EQUAL("2 3 4", str_c->join(' '));
  CPPUNIT_ASSERT_EQUAL(size_t(3), str_c->size());

  (*str_b)[0] = "5";
  (*str_b)[1] = "6";
  (*str_c) += (*str_b);

  CPPUNIT_ASSERT_STRING_EQUAL("2 3 4 5 6", str_c->join(' '));
  CPPUNIT_ASSERT_EQUAL(size_t(5), str_c->size());

  (*str_b) = (*str_c)--;

  CPPUNIT_ASSERT_STRING_EQUAL("6", str_b->join(' '));
  CPPUNIT_ASSERT_STRING_EQUAL("2 3 4 5", str_c->join(' '));
  CPPUNIT_ASSERT_EQUAL(size_t(1), str_b->size());
  CPPUNIT_ASSERT_EQUAL(size_t(4), str_c->size());

  --(*str_c);

  CPPUNIT_ASSERT_STRING_EQUAL("2 3 4", str_c->join(' '));
  CPPUNIT_ASSERT_EQUAL(size_t(3), str_c->size());

  ++(*str_c);
  CPPUNIT_ASSERT_STRING_EQUAL("3 4", str_c->join(' '));
  CPPUNIT_ASSERT_EQUAL(size_t(2), str_c->size());

  (*str_b) = (*str_c)++;
  CPPUNIT_ASSERT_EQUAL(size_t(1), str_c->size());
  CPPUNIT_ASSERT_STRING_EQUAL("3", str_b->join(' '));
  CPPUNIT_ASSERT_STRING_EQUAL("4", str_c->join(' '));

  (*str_b) = Array<String>{"Test0", "Test1"}++;
  CPPUNIT_ASSERT_EQUAL(size_t(1), str_b->size());
  CPPUNIT_ASSERT_STRING_EQUAL("Test0", str_b->join(' '));

  (*str_b) = Array<String>{"Test0", "Test1"}--;
  CPPUNIT_ASSERT_EQUAL(size_t(1), str_b->size());
  CPPUNIT_ASSERT_STRING_EQUAL("Test1", str_b->join(' '));
}

void ArrayTest :: initializerTest(void) {
  // Test initializer constructor
  Array<int> my_array_initializer = {0, 1, 2, 3};
  CPPUNIT_ASSERT_EQUAL(size_t(4), my_array_initializer.size());
  CPPUNIT_ASSERT_EQUAL(0, static_cast<int>(my_array_initializer[0]));
  CPPUNIT_ASSERT_EQUAL(1, static_cast<int>(my_array_initializer[1]));
  CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(my_array_initializer[2]));
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(my_array_initializer[3]));

  // Test initializer assignment
  *str_a = {"0", "1", "2", "3"};
  CPPUNIT_ASSERT_EQUAL(size_t(4), str_a->size());
  CPPUNIT_ASSERT_STRING_EQUAL("0", (*str_a)[0]);
  CPPUNIT_ASSERT_STRING_EQUAL("1", (*str_a)[1]);
  CPPUNIT_ASSERT_STRING_EQUAL("2", (*str_a)[2]);
  CPPUNIT_ASSERT_STRING_EQUAL("3", (*str_a)[3]);
}

void ArrayTest :: iteratorTest(void)
{
  str_a->push_back("1");
  str_a->push_back("2");
  str_a->push_back("3");
  str_a->push_back("4");

  auto it = str_a->begin();
  CPPUNIT_ASSERT_EQUAL(false, it == str_a->end());
  CPPUNIT_ASSERT_STRING_EQUAL("1", *it);
  ++it;
  CPPUNIT_ASSERT_EQUAL(false, it == str_a->end());
  CPPUNIT_ASSERT_STRING_EQUAL("2", *it);
  ++it;
  CPPUNIT_ASSERT_EQUAL(false, it == str_a->end());
  CPPUNIT_ASSERT_STRING_EQUAL("3", *it);
  ++it;
  CPPUNIT_ASSERT_EQUAL(false, it == str_a->end());
  CPPUNIT_ASSERT_STRING_EQUAL("4", *it);
  ++it;
  CPPUNIT_ASSERT_EQUAL(true, it == str_a->end());

  str_c = new Array<String>;
  *str_c = *str_b = *str_a;
  CPPUNIT_ASSERT_EQUAL(size_t(3), str_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(3), str_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(3), str_c->rcount());
  std::copy(str_a->cbegin(), str_a->cend(),
      std::back_inserter((*str_c)));
  CPPUNIT_ASSERT_EQUAL(size_t(2), str_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(2), str_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), str_c->rcount());
  CPPUNIT_ASSERT_EQUAL(false, str_c->isEmpty());
  CPPUNIT_ASSERT_EQUAL(size_t(8), str_c->length());
  CPPUNIT_ASSERT_STRING_EQUAL("1", (*str_c)[0]);
  CPPUNIT_ASSERT_STRING_EQUAL("2", (*str_c)[1]);
  CPPUNIT_ASSERT_STRING_EQUAL("3", (*str_c)[2]);
  CPPUNIT_ASSERT_STRING_EQUAL("4", (*str_c)[3]);
  CPPUNIT_ASSERT_STRING_EQUAL("1", (*str_c)[4]);
  CPPUNIT_ASSERT_STRING_EQUAL("2", (*str_c)[5]);
  CPPUNIT_ASSERT_STRING_EQUAL("3", (*str_c)[6]);
  CPPUNIT_ASSERT_STRING_EQUAL("4", (*str_c)[7]);

  *str_c = *str_b = *str_a;
  CPPUNIT_ASSERT_EQUAL(size_t(3), str_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(3), str_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(3), str_c->rcount());
  std::copy(str_a->cbegin(), str_a->cend(),
      std::inserter((*str_c), std::next((*str_c).begin(), 1)));
  CPPUNIT_ASSERT_EQUAL(size_t(2), str_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(2), str_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), str_c->rcount());
  CPPUNIT_ASSERT_EQUAL(false, str_c->isEmpty());
  CPPUNIT_ASSERT_EQUAL(size_t(8), str_c->length());
  CPPUNIT_ASSERT_STRING_EQUAL("1", (*str_c)[0]);
  CPPUNIT_ASSERT_STRING_EQUAL("1", (*str_c)[1]);
  CPPUNIT_ASSERT_STRING_EQUAL("2", (*str_c)[2]);
  CPPUNIT_ASSERT_STRING_EQUAL("3", (*str_c)[3]);
  CPPUNIT_ASSERT_STRING_EQUAL("4", (*str_c)[4]);
  CPPUNIT_ASSERT_STRING_EQUAL("2", (*str_c)[5]);
  CPPUNIT_ASSERT_STRING_EQUAL("3", (*str_c)[6]);
  CPPUNIT_ASSERT_STRING_EQUAL("4", (*str_c)[7]);

  *str_c = *str_a;
  (*str_a).clear();
  str_a->push_back("A");
  str_a->push_back("B");
  str_a->push_back("C");
  str_a->push_back("D");
  *str_b = *str_a;
  CPPUNIT_ASSERT_EQUAL(size_t(2), str_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(2), str_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), str_c->rcount());
  std::copy(str_a->cbegin(), str_a->cend(),
      std::front_inserter((*str_c)));
  CPPUNIT_ASSERT_EQUAL(size_t(2), str_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(2), str_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), str_c->rcount());
  CPPUNIT_ASSERT_EQUAL(false, str_c->isEmpty());
  CPPUNIT_ASSERT_EQUAL(size_t(8), str_c->length());
  CPPUNIT_ASSERT_STRING_EQUAL("D", (*str_c)[0]);
  CPPUNIT_ASSERT_STRING_EQUAL("C", (*str_c)[1]);
  CPPUNIT_ASSERT_STRING_EQUAL("B", (*str_c)[2]);
  CPPUNIT_ASSERT_STRING_EQUAL("A", (*str_c)[3]);
  CPPUNIT_ASSERT_STRING_EQUAL("1", (*str_c)[4]);
  CPPUNIT_ASSERT_STRING_EQUAL("2", (*str_c)[5]);
  CPPUNIT_ASSERT_STRING_EQUAL("3", (*str_c)[6]);
  CPPUNIT_ASSERT_STRING_EQUAL("4", (*str_c)[7]);

  Array<String> str_d(str_c->cbegin(), str_c->cend());
  CPPUNIT_ASSERT_ARRAY_EQUAL((*str_c), str_d);
  CPPUNIT_ASSERT_EQUAL(size_t(8), str_c->length());
  CPPUNIT_ASSERT_STRING_EQUAL("D", (*str_c)[0]);
  CPPUNIT_ASSERT_STRING_EQUAL("C", (*str_c)[1]);
  CPPUNIT_ASSERT_STRING_EQUAL("B", (*str_c)[2]);
  CPPUNIT_ASSERT_STRING_EQUAL("A", (*str_c)[3]);
  CPPUNIT_ASSERT_STRING_EQUAL("1", (*str_c)[4]);
  CPPUNIT_ASSERT_STRING_EQUAL("2", (*str_c)[5]);
  CPPUNIT_ASSERT_STRING_EQUAL("3", (*str_c)[6]);
  CPPUNIT_ASSERT_STRING_EQUAL("4", (*str_c)[7]);

  std::vector<String> v_a;
  v_a.push_back("A");
  v_a.push_back("B");
  v_a.push_back("C");
  v_a.push_back("D");
  Array<String> str_e(
      std::make_move_iterator(v_a.begin()),
      std::make_move_iterator(v_a.end()));
  CPPUNIT_ASSERT_STRING_EQUAL("A", str_e[0]);
  CPPUNIT_ASSERT_STRING_EQUAL("B", str_e[1]);
  CPPUNIT_ASSERT_STRING_EQUAL("C", str_e[2]);
  CPPUNIT_ASSERT_STRING_EQUAL("D", str_e[3]);
  CPPUNIT_ASSERT_EQUAL(size_t(4), str_e.length());
  CPPUNIT_ASSERT_EQUAL(size_t(0), v_a[0].length());
  CPPUNIT_ASSERT_EQUAL(size_t(0), v_a[1].length());
  CPPUNIT_ASSERT_EQUAL(size_t(0), v_a[2].length());
  CPPUNIT_ASSERT_EQUAL(size_t(0), v_a[3].length());
  CPPUNIT_ASSERT_EQUAL(size_t(4), v_a.size());

  (*str_a).clear();
  str_a->push_back("A");
  str_a->push_back("B");
  str_a->push_back("C");
  str_a->push_back("D");
  Array<String> str_f(
      std::make_move_iterator(str_a->begin()),
      std::make_move_iterator(str_a->end()));
  CPPUNIT_ASSERT_STRING_EQUAL("A", str_f[0]);
  CPPUNIT_ASSERT_STRING_EQUAL("B", str_f[1]);
  CPPUNIT_ASSERT_STRING_EQUAL("C", str_f[2]);
  CPPUNIT_ASSERT_STRING_EQUAL("D", str_f[3]);
  CPPUNIT_ASSERT_EQUAL(size_t(4), str_f.length());
  CPPUNIT_ASSERT_EQUAL(true, (*str_a)[0].get().isEmpty());
  CPPUNIT_ASSERT_EQUAL(size_t(0), (*str_a)[0].get().rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(0), (*str_a)[0].get().length());
  CPPUNIT_ASSERT_EQUAL(size_t(0), (*str_a)[1].get().length());
  CPPUNIT_ASSERT_EQUAL(size_t(0), (*str_a)[2].get().length());
  CPPUNIT_ASSERT_EQUAL(size_t(0), (*str_a)[3].get().length());
  CPPUNIT_ASSERT_EQUAL(size_t(4), str_a->size());
  (*str_a)[0] = "what";
  CPPUNIT_ASSERT_STRING_EQUAL("A", str_f[0]);
  CPPUNIT_ASSERT_STRING_EQUAL("what", (*str_a)[0]);
}

/* vim: set sts=2 sw=2 ts=8 et: */
