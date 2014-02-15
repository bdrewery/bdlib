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
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
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
    delete int_a; delete int_b; delete int_c;
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
  str_a->push(String("Test1"));
  (*str_a) << String("Test2");
  CPPUNIT_ASSERT_EQUAL(size_t(2), str_a->size());
  CPPUNIT_ASSERT(str_a->capacity() >= 2);
  CPPUNIT_ASSERT_STRING_EQUAL("Test2", str_a->pop());
  CPPUNIT_ASSERT_STRING_EQUAL("Test1", str_a->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), str_a->size());
  CPPUNIT_ASSERT(str_a->capacity() >= 2);

  String a = "Testa";
  String b = "Testb";
  String c = b;
  str_a->push(a);

  str_a->push(a);
  str_a->pop();

  str_a->push(b);
  str_a->push(a);
  str_a->push(b);
  str_a->push(b);
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

  CPPUNIT_ASSERT_STRING_EQUAL(String(), str_a->pop());

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

  (*str_a) = Array<String>();
  (*str_a) << "Test";
  (*str_a) << "1 2 3";
  (*str_a) << "Last 1";
  CPPUNIT_ASSERT_STRING_EQUAL("\"Test\" \"1 2 3\" \"Last 1\"", str_a->join(" ", true));
}

void ArrayTest :: arrayConsTest(void)
{
  String carray2[] = {"2Test1", "2Test2"};
  str_c = new Array<String>(carray2, 2);
  CPPUNIT_ASSERT_EQUAL(size_t(2), str_c->size());
  CPPUNIT_ASSERT_STRING_EQUAL("2Test2", str_c->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(1), str_c->size());
  CPPUNIT_ASSERT_STRING_EQUAL("2Test1", str_c->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), str_c->size());

  int carray[] = {1, 2, 0};
  Array<int>* int_d = new Array<int>(carray);
  CPPUNIT_ASSERT_EQUAL(size_t(2), int_d->size());
  CPPUNIT_ASSERT_EQUAL(2, int_d->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(1), int_d->size());
  CPPUNIT_ASSERT_EQUAL(1, int_d->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), int_d->size());
  delete int_d;
}

void ArrayTest :: clearTest (void)
{
  for (int i = 0; i < 10; ++i)
    int_a->push(i);
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_a->size());
  int_a->clear();
  CPPUNIT_ASSERT_EQUAL(size_t(0), int_a->size());
  CPPUNIT_ASSERT_EQUAL(true, int_a->isEmpty());
}


void ArrayTest :: refTest (void)
{
  for (int i = 0; i < 10; ++i)
    int_a->push(i);
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_a->size());
  CPPUNIT_ASSERT_EQUAL(size_t(1), int_a->rcount());
  *int_b = *int_a;
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_a->size());
  CPPUNIT_ASSERT_EQUAL(size_t(2), int_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_b->size());
  CPPUNIT_ASSERT_EQUAL(size_t(2), int_b->rcount());

  *int_c = *int_a;
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_a->size());
  CPPUNIT_ASSERT_EQUAL(size_t(3), int_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_b->size());
  CPPUNIT_ASSERT_EQUAL(size_t(3), int_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_c->size());
  CPPUNIT_ASSERT_EQUAL(size_t(3), int_c->rcount());

  int_b->push(99);
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_a->size());
  CPPUNIT_ASSERT_EQUAL(size_t(2), int_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(11), int_b->size());
  CPPUNIT_ASSERT_EQUAL(size_t(1), int_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_c->size());
  CPPUNIT_ASSERT_EQUAL(size_t(2), int_c->rcount());

  for (int i = 9; i >= 0; --i)
    CPPUNIT_ASSERT_EQUAL(i, int_a->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), int_a->size());
//  CPPUNIT_ASSERT_EQUAL(size_t(2), int_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(11), int_b->size());
//  CPPUNIT_ASSERT_EQUAL(size_t(1), int_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_c->size());
//  CPPUNIT_ASSERT_EQUAL(size_t(2), int_c->rcount());

  CPPUNIT_ASSERT_EQUAL(99, int_b->pop());
  for (int i = 9; i > 0; --i)
    CPPUNIT_ASSERT_EQUAL(i, int_b->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), int_a->size());
//  CPPUNIT_ASSERT_EQUAL(size_t(2), int_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), int_b->size());
//  CPPUNIT_ASSERT_EQUAL(size_t(1), int_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_c->size());
//  CPPUNIT_ASSERT_EQUAL(size_t(1), int_c->rcount());

  *int_b = *int_c;
  CPPUNIT_ASSERT_EQUAL(size_t(0), int_a->size());
//  CPPUNIT_ASSERT_EQUAL(size_t(3), int_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_b->size());
//  CPPUNIT_ASSERT_EQUAL(size_t(2), int_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_c->size());
//  CPPUNIT_ASSERT_EQUAL(size_t(2), int_c->rcount());
}

void ArrayTest :: joinTest(void)
{
  str_a->push("Test1");
  str_a->push("Test2");
  str_a->push("Test3");
  CPPUNIT_ASSERT_STRING_EQUAL("Test1 Test2 Test3", str_a->join(' '));
  CPPUNIT_ASSERT_EQUAL(size_t(3), str_a->size());
  CPPUNIT_ASSERT_STRING_EQUAL("Test3", str_a->pop());
  CPPUNIT_ASSERT_STRING_EQUAL("Test2", str_a->pop());
  CPPUNIT_ASSERT_STRING_EQUAL("Test1", str_a->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), str_a->size());
}

void ArrayTest :: compareTest(void)
{
  str_a->push("Test1");
  str_a->push("Test2");

  str_b->push("Test1");
  CPPUNIT_ASSERT_EQUAL(false, (*str_a) == (*str_b));
  str_b->push("Test2");

  CPPUNIT_ASSERT_EQUAL(true, (*str_a) == (*str_b));
}

void ArrayTest :: findTest(void)
{
  str_a->push("Test1");
  str_a->push("Test2");
  CPPUNIT_ASSERT_EQUAL(size_t(-1), str_a->find("Test3"));
  CPPUNIT_ASSERT_EQUAL(size_t(0), str_a->find("Test1"));
  CPPUNIT_ASSERT_EQUAL(size_t(1), str_a->find("Test2"));
}

void ArrayTest :: indexTest(void)
{
  str_a->push("Test1");
  str_a->push("Test2");
  CPPUNIT_ASSERT_STRING_EQUAL("Test1", (*str_a)[0]);
  CPPUNIT_ASSERT_STRING_EQUAL("Test2", (*str_a)[1]);

  (*str_b) = (*str_a);

  (*str_a)[1] = "Test3";
  CPPUNIT_ASSERT_STRING_EQUAL("Test1", (*str_a)[0]);
  CPPUNIT_ASSERT_STRING_EQUAL("Test3", (*str_a)[1]);

  CPPUNIT_ASSERT_STRING_EQUAL("Test1", (*str_b)[0]);
  CPPUNIT_ASSERT_STRING_EQUAL("Test2", (*str_b)[1]);
}

void ArrayTest :: subArrayTest (void)
{
  str_a->push("Test0");
  str_a->push("Test1");
  str_a->push("Test2");
  str_a->push("Test3");
  str_a->push("Test4");
  str_a->push("Test5");
  str_a->push("Test6");
  str_a->push("Test7");

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
  inplaceTest.push("TEST0");
  inplaceTest.push("TEST1");
  inplaceTest.push("TEST2");
  inplaceTest.push("TEST3");
  (*str_b) = (*str_a);
  (*str_b)(0, 2) = inplaceTest;

  Array<String> expectedResults(*str_a);
  expectedResults[0] = "TEST0";
  expectedResults[1] = "TEST1";
  CPPUNIT_ASSERT_EQUAL(size_t(8), str_a->size());
  CPPUNIT_ASSERT_EQUAL(size_t(8), str_b->size());
  CPPUNIT_ASSERT_ARRAY_EQUAL((*str_b), expectedResults);
}


void ArrayTest :: hashTest(void)
{
  str_c = new Array<String>();

  (*str_a).push("test");
  (*str_a).push("test2");
  (*str_b).push("test");
  (*str_b).push("test2");
  (*str_c) = (*str_b);
  CPPUNIT_ASSERT_EQUAL(str_a->hash(), str_b->hash());
  CPPUNIT_ASSERT_EQUAL(str_a->hash(), str_c->hash());
  CPPUNIT_ASSERT_EQUAL(str_b->hash(), str_c->hash());

  str_a->clear();
  str_b->clear();
  (*str_a).push("test");
  (*str_a).push("test1");
  (*str_b).push("test");
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
  str_a->push("1");
  str_a->push("2");
  str_b->push("3");
  str_b->push("4");

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
