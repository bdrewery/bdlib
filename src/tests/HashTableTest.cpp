/* HashTableTest.c
 *
 * $Id$
 */
#include "HashTableTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (HashTableTest);

void HashTableTest :: setUp (void)
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
  a = new HashTable<int, String>();
}

void HashTableTest :: tearDown (void)
{
    // finally delete objects
    delete a; 
/*
    delete b; delete c; delete d;
    delete e; delete f; delete g; delete h;
*/
}

void HashTableTest :: insertTest (void)
{
  CPPUNIT_ASSERT_EQUAL((size_t)0, a->size());
  a->insert(1, "Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)1, a->size());
  a->insert(1, "Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)1, a->size());
  a->insert(2, "Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)2, a->size());
  a->insert(8, "Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)3, a->size());
  a->insert(4, "Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)4, a->size());
}

void HashTableTest :: containsTest (void)
{
  a->insert(1, "Blah");
  a->insert(1, "Bleck");
  a->insert(2, "Blah");
  a->insert(8, "Blah");
  a->insert(4, "Blah");
  
  CPPUNIT_ASSERT_EQUAL(true, a->contains(1));
  CPPUNIT_ASSERT_EQUAL(false, a->contains(3));
  CPPUNIT_ASSERT_EQUAL(true, a->contains(4));
}

void HashTableTest :: getValueTest (void)
{
  a->insert(1, "Blah1");
  CPPUNIT_ASSERT_STRING_EQUAL("Blah1", a->getValue(1));
  CPPUNIT_ASSERT_EQUAL((size_t)1, a->size());

  CPPUNIT_ASSERT_EQUAL(false, a->insert(1, "Bleck"));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah1", a->getValue(1));
  CPPUNIT_ASSERT_EQUAL((size_t)1, a->size());


  a->insert(2, "Blah2");
  CPPUNIT_ASSERT_STRING_EQUAL("Blah1", a->getValue(1));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah2", a->getValue(2));
  CPPUNIT_ASSERT_EQUAL((size_t)2, a->size());

  a->insert(8, "Blah8");
  CPPUNIT_ASSERT_STRING_EQUAL("Blah1", a->getValue(1));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah2", a->getValue(2));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah8", a->getValue(8));
  CPPUNIT_ASSERT_EQUAL((size_t)3, a->size());

  a->insert(4, "Blah4");
  CPPUNIT_ASSERT_STRING_EQUAL("Blah4", a->getValue(4));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah1", a->getValue(1));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah2", a->getValue(2));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah8", a->getValue(8));
  CPPUNIT_ASSERT_EQUAL((size_t)4, a->size());

  
  String result = a->getValue(58);
  CPPUNIT_ASSERT_EQUAL(true, result.isEmpty());
}

#ifdef DISABLED
void HashTableTest :: getKeyTest (void)
{
  a->insert(1, "Blah");
  a->insert(1, "Bleck");
  a->insert(2, "Blah2");
  a->insert(8, "Blah8");
  a->insert(4, "Blah4");
  
  CPPUNIT_ASSERT_EQUAL(1, a->getKey("Blah"));
  CPPUNIT_ASSERT_EQUAL(2, a->getKey("Blah2"));
  CPPUNIT_ASSERT_EQUAL(8, a->getKey("Blah8"));
  CPPUNIT_ASSERT_EQUAL(4, a->getKey("Blah4"));
}
#endif

void HashTableTest :: removeTest (void)
{
  a->insert(1, "Blah");
  a->insert(1, "Bleck");
  a->insert(2, "Blah2");
  a->insert(8, "Blah8");
  a->insert(4, "Blah4");

  a->remove(8);  

  String result;

  result = a->getValue(58);
  CPPUNIT_ASSERT_EQUAL(true, result.isEmpty());

 
  CPPUNIT_ASSERT_STRING_EQUAL("Blah", a->getValue(1));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah4", a->getValue(4));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah2", a->getValue(2));
  result = a->getValue(8);
  CPPUNIT_ASSERT_EQUAL(true, result.isEmpty());

  CPPUNIT_ASSERT_EQUAL(false, a->remove(8));

  a->remove(4);
  CPPUNIT_ASSERT_STRING_EQUAL("Blah", a->getValue(1));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah2", a->getValue(2));
  result = a->getValue(4);
  CPPUNIT_ASSERT_EQUAL(true, result.isEmpty());

  a->remove(2);
  CPPUNIT_ASSERT_STRING_EQUAL("Blah", a->getValue(1));
  result = a->getValue(2);
  CPPUNIT_ASSERT_EQUAL(true, result.isEmpty());

  a->remove(1);
  CPPUNIT_ASSERT_EQUAL(true, a->isEmpty());
  result = a->getValue(1);
  CPPUNIT_ASSERT_EQUAL(true, result.isEmpty());
}

#ifdef disabled
void HashTableTest :: iterateTest (void)
{
  a->insert(1, "Blah");
  a->insert(1, "Bleck");
  a->insert(2, "Blah2");
  a->insert(8, "Blah8");
  a->insert(4, "Blah4");

  HashTable<int, String>::iterator iter = a->begin();
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
}
#endif
