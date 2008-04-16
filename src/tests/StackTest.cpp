/* StackTest.c
 *
 * $Id$
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
