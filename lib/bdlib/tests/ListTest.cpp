/* ListTest.c
 *
 * $Id$
 */
#include "ListTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (ListTest);

void ListTest :: setUp (void)
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
  a = new List<String>();
  b = new List<String>();
  c = new List<String>();
  d = new List<String>();
  e = new List<String*>();
  f = new List<String*>();
  g = new List<String*>();
  h = new List<String*>();
}

void ListTest :: tearDown (void)
{
    // finally delete objects
    delete a; delete b; delete c; delete d;
    delete e; delete f; delete g; delete h;
}

void ListTest :: insertTest (void)
{
  CPPUNIT_ASSERT_EQUAL((size_t)0, a->size());
  a->insert("Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)1, a->size());
  a->insert("Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)2, a->size());
  a->insert("Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)3, a->size());
  a->insert("Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)4, a->size());
  a->insert("Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)5, a->size());
}

void ListTest :: containsTest (void)
{
  a->insert("Blahstart");
  a->insert("Bleck");
  a->insert("Blah");
  a->insert("Blah");
  a->insert("Blahend");

  CPPUNIT_ASSERT_EQUAL((size_t)5, a->size());  
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blahstart"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Bleck"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blahend"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blahk"));
}

void ListTest :: copyTest (void)
{
  a->insert("Blahstart");
  a->insert("Bleck");
  a->insert("Blah");
  a->insert("Blah");
  a->insert("Blahend");

  CPPUNIT_ASSERT_EQUAL((size_t)5, a->size());  
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blahstart"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Bleck"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blahend"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blahk"));

  delete b;
  b = new List<String>(*a);
  CPPUNIT_ASSERT_EQUAL((size_t)5, b->size());  
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blahstart"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Bleck"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blahend"));
  CPPUNIT_ASSERT_EQUAL(false, b->contains("Blahk"));

  delete b;
  b = new List<String>();
  (*b) = (*a);
  CPPUNIT_ASSERT_EQUAL((size_t)5, b->size());  
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blahstart"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Bleck"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blahend"));
  CPPUNIT_ASSERT_EQUAL(false, b->contains("Blahk"));

  (*b) = (*b);
  CPPUNIT_ASSERT_EQUAL((size_t)5, b->size());  
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blahstart"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Bleck"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blahend"));
  CPPUNIT_ASSERT_EQUAL(false, b->contains("Blahk"));
}

void ListTest :: clearTest (void)
{
  a->insert("Blahstart");
  a->insert("Bleck");
  a->insert("Blah");
  a->insert("Blah");
  a->insert("Blahend");

  CPPUNIT_ASSERT_EQUAL((size_t)5, a->size());  
  CPPUNIT_ASSERT_EQUAL(false, a->isEmpty());

  a->clear();

  CPPUNIT_ASSERT_EQUAL((size_t)0, a->size());  
  CPPUNIT_ASSERT_EQUAL(true, a->isEmpty());
}

void ListTest :: removeTest (void)
{
  a->insert("Blahstart");
  a->insert("Bleck");
  a->insert("Blah2");
  a->insert("Blah2");
  a->insert("Blah8");
  a->insert("Blahend");

  CPPUNIT_ASSERT_EQUAL((size_t)6, a->size());
  CPPUNIT_ASSERT_EQUAL(false, a->isEmpty());

  a->remove("Blahend");
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blahstart"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blackend"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Bleck"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah2"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah8"));

  CPPUNIT_ASSERT_EQUAL((size_t)5, a->size());
  CPPUNIT_ASSERT_EQUAL(false, a->isEmpty());

  a->remove("Bleck");
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blahstart"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blackend"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Bleck"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah2"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah8"));

  CPPUNIT_ASSERT_EQUAL((size_t)4, a->size());
  CPPUNIT_ASSERT_EQUAL(false, a->isEmpty());

  a->remove("Blah2");
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blahstart"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blackend"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Bleck"));
  /* 2 were added! */
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah2"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah8"));

  CPPUNIT_ASSERT_EQUAL((size_t)3, a->size());
  CPPUNIT_ASSERT_EQUAL(false, a->isEmpty());

  a->remove("Blah8");
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blahstart"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blackend"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Bleck"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah2"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah8"));

  CPPUNIT_ASSERT_EQUAL((size_t)2, a->size());
  CPPUNIT_ASSERT_EQUAL(false, a->isEmpty());

  a->remove("Blah2");
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blahstart"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blackend"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Bleck"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah2"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah8"));

  CPPUNIT_ASSERT_EQUAL((size_t)1, a->size());
  CPPUNIT_ASSERT_EQUAL(false, a->isEmpty());

  a->remove("Blahstart");
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blahstart"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blackend"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Bleck"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah2"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah8"));

  CPPUNIT_ASSERT_EQUAL((size_t)0, a->size());
  CPPUNIT_ASSERT_EQUAL(true, a->isEmpty());
}

void ListTest :: iterateTest (void)
{
#ifdef NO
  a->insert("Blah");
  a->insert("Bleck");
  a->insert("Blah2");
  a->insert("Blah8");
  a->insert("Blah4");

  List<int, String>::iterator iter = a->begin();
  while (iter.hasNext()) {
//    int key = (int) iter.next();
//    printf("%d\n", key);
    String value = iter.next();
//    printf("%d: %s\n", key, a->getValue(key));
  }

  iter = a->begin();
  CPPUNIT_ASSERT_STRING_EQUAL("Blah", iter.next());
  CPPUNIT_ASSERT_STRING_EQUAL("Blah2", iter.next());
  CPPUNIT_ASSERT_STRING_EQUAL("Blah4", iter.next());
  CPPUNIT_ASSERT_STRING_EQUAL("Blah8", iter.next());

  a->remove(4);
  iter = a->begin();
  CPPUNIT_ASSERT_STRING_EQUAL("Blah", iter.next());
  CPPUNIT_ASSERT_STRING_EQUAL("Blah2", iter.next());
  CPPUNIT_ASSERT_STRING_EQUAL("Blah8", iter.next());

  a->remove(2);
  iter = a->begin();
  CPPUNIT_ASSERT_STRING_EQUAL("Blah", iter.next());
  CPPUNIT_ASSERT_STRING_EQUAL("Blah8", iter.next());

  a->remove(8);
  iter = a->begin();
  CPPUNIT_ASSERT_STRING_EQUAL("Blah", iter.next());

  a->remove(1);
  iter = a->begin();
  CPPUNIT_ASSERT_EQUAL(false, iter.hasNext());
#endif
}
