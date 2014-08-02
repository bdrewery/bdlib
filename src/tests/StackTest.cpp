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

/* StackTest.c
 *
 */
#include "StackTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (StackTest);

void StackTest :: setUp (void)
{
/*
    // set up test environment (initializing objects)
    a = new String();
    b = new String("blah");
    c = new String(*b);
    d = new String(cstring);
    e = new String(*d);
    f = new String(cstring, 11);
    g = new String('x');
    h = new String(35);
*/
  a = new Stack<String>();
  b = new Stack<String>();
  c = new Stack<String>();
  d = new Stack<String>();
  e = new Stack<String*>();
  f = new Stack<String*>();
  g = new Stack<String*>();
  h = new Stack<String*>();
}

void StackTest :: tearDown (void)
{
    // finally delete objects
    delete a; delete b; delete c; delete d;
    delete e; delete f; delete g; delete h;
}

void StackTest :: pushTest (void)
{
  CPPUNIT_ASSERT_EQUAL((size_t)0, a->size());
  a->push("Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)1, a->size());
  a->push("Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)2, a->size());
  a->push("Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)3, a->size());
  a->push("Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)4, a->size());
  a->push("Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)5, a->size());

  (*a) << "Blah1" << "Blah2";
  CPPUNIT_ASSERT_EQUAL((size_t)7, a->size());
}

void StackTest :: popTest (void)
{
  a->push("Blahstart");
  a->push("Bleck");
  a->push("Blah");
  a->push("Blah");
  a->push("Blahend");

  CPPUNIT_ASSERT_EQUAL((size_t)5, a->size());
  CPPUNIT_ASSERT_STRING_EQUAL("Blahend", a->pop());

  CPPUNIT_ASSERT_EQUAL((size_t)4, a->size());

  CPPUNIT_ASSERT_STRING_EQUAL("Blah", a->peek());
  CPPUNIT_ASSERT_EQUAL((size_t)4, a->size());

  CPPUNIT_ASSERT_STRING_EQUAL("Blah", a->peek());
  CPPUNIT_ASSERT_EQUAL((size_t)4, a->size());

  CPPUNIT_ASSERT_STRING_EQUAL("Blah", a->pop());
  
  CPPUNIT_ASSERT_EQUAL((size_t)3, a->size());
  CPPUNIT_ASSERT_STRING_EQUAL("Blah", a->pop());
  
  CPPUNIT_ASSERT_EQUAL((size_t)2, a->size());
  CPPUNIT_ASSERT_STRING_EQUAL("Bleck", a->pop());
  
  CPPUNIT_ASSERT_EQUAL((size_t)1, a->size());
  CPPUNIT_ASSERT_STRING_EQUAL("Blahstart", a->pop());
  
  CPPUNIT_ASSERT_EQUAL((size_t)0, a->size());

  (*a) << "One" << "Two" << "Three";
  String one, two, three;
  (*a) >> three >> two;
  CPPUNIT_ASSERT_EQUAL((size_t)1, a->size());
  CPPUNIT_ASSERT_STRING_EQUAL("Three", three);
  CPPUNIT_ASSERT_STRING_EQUAL("Two", two);
  (*a) >> one;
  CPPUNIT_ASSERT_EQUAL((size_t)0, a->size());
  CPPUNIT_ASSERT_STRING_EQUAL("One", one);
}

void StackTest :: iterateTest (void)
{
  (*a) << "1" << "2" << "3" << "4";
  Stack<String>::iterator iter(a->begin());
  int count = 4;
  for (; iter; ++iter) {
    char buf[2] = "";
    sprintf(buf, "%d", count);
    --count;
    CPPUNIT_ASSERT_STRING_EQUAL(buf, a->pop());
  }
}
/* vim: set sts=2 sw=2 ts=8 et: */
