/* ArrayTest.c
 *
 */
#include "ArrayTest.h"
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
  CPPUNIT_ASSERT_EQUAL(size_t(1), int_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(11), int_b->size());
  CPPUNIT_ASSERT_EQUAL(size_t(1), int_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_c->size());
  CPPUNIT_ASSERT_EQUAL(size_t(1), int_c->rcount());

  CPPUNIT_ASSERT_EQUAL(99, int_b->pop());
  for (int i = 9; i > 0; --i)
    CPPUNIT_ASSERT_EQUAL(i, int_b->pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), int_a->size());
  CPPUNIT_ASSERT_EQUAL(size_t(1), int_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), int_b->size());
  CPPUNIT_ASSERT_EQUAL(size_t(1), int_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_c->size());
  CPPUNIT_ASSERT_EQUAL(size_t(1), int_c->rcount());

  *int_b = *int_c;
  CPPUNIT_ASSERT_EQUAL(size_t(0), int_a->size());
  CPPUNIT_ASSERT_EQUAL(size_t(1), int_a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_b->size());
  CPPUNIT_ASSERT_EQUAL(size_t(2), int_b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(10), int_c->size());
  CPPUNIT_ASSERT_EQUAL(size_t(2), int_c->rcount());
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
