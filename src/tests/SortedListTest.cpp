/* SortedListTest.c
 *
 */
#include "SortedListTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (SortedListTest);

void SortedListTest :: setUp (void)
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
  a = new SortedList<String>();
  b = new SortedList<String>();
  c = new SortedList<String>();
  d = new SortedList<String>();
  e = new SortedList<String*>();
  f = new SortedList<String*>();
  g = new SortedList<String*>();
  h = new SortedList<String*>();
}

void SortedListTest :: tearDown (void)
{
    // finally delete objects
    delete a; delete b; delete c; delete d;
    delete e; delete f; delete g; delete h;
}

void SortedListTest :: insertTest (void)
{
  CPPUNIT_ASSERT_EQUAL((size_t)0, a->size());
  a->insert("Blah1");
  CPPUNIT_ASSERT_EQUAL((size_t)1, a->size());
  a->insert("Blah2");
  CPPUNIT_ASSERT_EQUAL((size_t)2, a->size());
  a->insert("Blah3");
  CPPUNIT_ASSERT_EQUAL((size_t)3, a->size());
  a->insert("Blah4");
  CPPUNIT_ASSERT_EQUAL((size_t)4, a->size());
  a->insert("Blah5");
  CPPUNIT_ASSERT_EQUAL((size_t)5, a->size());

  (*a) << "Blah6";
  CPPUNIT_ASSERT_EQUAL((size_t)6, a->size());
  (*a) << "Blah7" << "Blah8";
  CPPUNIT_ASSERT_EQUAL((size_t)8, a->size());
}

void SortedListTest :: containsTest (void)
{
  a->insert("Blah2");
  a->insert("Blah5");
  a->insert("Blah3");
  a->insert("Blah4");
  a->insert("Blah1");
  (*a) << "Blah10" << "Blah12";

  CPPUNIT_ASSERT_EQUAL((size_t)7, a->size());  
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah2"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah5"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah3"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah4"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah1"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blahk"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah10"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah12"));
}

void SortedListTest :: copyTest (void)
{
  a->insert("Blah2");
  a->insert("Blah5");
  a->insert("Blah3");
  a->insert("Blah4");
  a->insert("Blah1");

  delete b;
  b = new SortedList<String>(*a);
  CPPUNIT_ASSERT_EQUAL((size_t)5, b->size());  
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah1"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah2"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah3"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah4"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah5"));
  CPPUNIT_ASSERT_EQUAL(false, b->contains("Blahk"));

  (*b) = (*a);
  CPPUNIT_ASSERT_EQUAL((size_t)5, b->size());  
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah1"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah2"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah3"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah4"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah5"));
  CPPUNIT_ASSERT_EQUAL(false, b->contains("Blahk"));

  (*b) = (*b);
  CPPUNIT_ASSERT_EQUAL((size_t)5, b->size());  
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah1"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah2"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah3"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah4"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah5"));
  CPPUNIT_ASSERT_EQUAL(false, b->contains("Blahk"));
}

void SortedListTest :: findTest(void)
{
  a->insert("Blah2");
  (*a) << "Blah5" << "Blah3";
  a->insert("Blah4");
  a->insert("Blah1");

  CPPUNIT_ASSERT_STRING_EQUAL("", a->find("WONT FIND THIS"));

  
  CPPUNIT_ASSERT_STRING_EQUAL("Blah1", a->find("Blah1"));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah2", a->find("Blah2"));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah3", a->find("Blah3"));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah4", a->find("Blah4"));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah5", a->find("Blah5"));

  a->remove("Blah2");

  CPPUNIT_ASSERT_STRING_EQUAL("Blah1", a->find("Blah1"));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah3", a->find("Blah3"));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah4", a->find("Blah4"));

  a->remove("Blah4");

  CPPUNIT_ASSERT_STRING_EQUAL("Blah1", a->find("Blah1"));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah3", a->find("Blah3"));

  a->remove("Blah1");

  CPPUNIT_ASSERT_STRING_EQUAL("Blah3", a->find("Blah3"));

  a->remove("Blah3");
  CPPUNIT_ASSERT_STRING_EQUAL("", a->find("WONT FIND THIS"));
}

void SortedListTest :: clearTest (void)
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

void SortedListTest :: removeTest (void)
{
  a->insert("Blah2");
  a->insert("Blah5");
  a->insert("Blah3");
  a->insert("Blah4");
  a->insert("Blah1");

  CPPUNIT_ASSERT_EQUAL((size_t)5, a->size());
  CPPUNIT_ASSERT_EQUAL(false, a->isEmpty());

  a->remove("Blah3");
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah1"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah2"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah3"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah4"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah5"));

  CPPUNIT_ASSERT_EQUAL((size_t)4, a->size());
  CPPUNIT_ASSERT_EQUAL(false, a->isEmpty());

  a->remove("Blah1");
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah1"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah2"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah3"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah4"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah5"));

  CPPUNIT_ASSERT_EQUAL((size_t)3, a->size());
  CPPUNIT_ASSERT_EQUAL(false, a->isEmpty());

  a->remove("Blah5");
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah1"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah2"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah3"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah4"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah5"));

 
  CPPUNIT_ASSERT_EQUAL((size_t)2, a->size());
  CPPUNIT_ASSERT_EQUAL(false, a->isEmpty());

  a->remove("Blah2");
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah1"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah2"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah3"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah4"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah5"));

  CPPUNIT_ASSERT_EQUAL((size_t)1, a->size());
  CPPUNIT_ASSERT_EQUAL(false, a->isEmpty());

  a->remove("Blah4");
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah1"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah2"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah3"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah4"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blah5"));

  CPPUNIT_ASSERT_EQUAL((size_t)0, a->size());
  CPPUNIT_ASSERT_EQUAL(true, a->isEmpty());
}

#ifdef NO
void SortedListTest :: iterateTest (void)
{
  a->insert("Blah1");
  a->insert("Blah2");
  a->insert("Blah3");
  a->insert("Blah4");
  a->insert("Blah5");

  SortedList<int, String>::iterator iter = a->begin();
  while (iter.hasNext()) {
//    int key = (int) iter.next();
//    printf("%d\n", key);
    String value = iter.next();
//    printf("%d: %s\n", key, a->getValue(key));
  }

  iter = a->begin();
  CPPUNIT_ASSERT_STRING_EQUAL("Blah1", iter.next());
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

void SortedListTest :: iterateTest (void)
{
  a->insert("Blah1");
  a->insert("Blah2");
  a->insert("Blah3");
  a->insert("Blah4");
  a->insert("Blah5");

  SortedList<String>::iterator iter = a->begin();
/*
  while (iter) {
//    int key = (int) iter.next();
//    printf("%d\n", key);
    String value = *(iter++);
//    printf("%d: %s\n", key, a->getValue(key));
  }
*/
  CPPUNIT_ASSERT_EQUAL(true, a->remove("Blah1"));

  iter = a->begin();

  CPPUNIT_ASSERT_STRING_EQUAL("Blah2", iter);
  
  CPPUNIT_ASSERT_STRING_EQUAL("Blah2", *(iter++));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah3", *(iter++));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah4", *(iter++));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah5", *(iter++));
  CPPUNIT_ASSERT_EQUAL(false, (bool)iter);

  CPPUNIT_ASSERT_EQUAL(true, a->remove("Blah3"));
  iter = a->begin();
  CPPUNIT_ASSERT_STRING_EQUAL("Blah2", *(iter++));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah4", *(iter++));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah5", *(iter++));
  CPPUNIT_ASSERT_EQUAL(false, (bool)iter);

  a->remove("Blah5");
  iter = a->begin();
  CPPUNIT_ASSERT_STRING_EQUAL("Blah4", (++iter));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah4", (iter));
  CPPUNIT_ASSERT_EQUAL(false, (bool) ++iter);

  a->remove("Blah2");
  iter = a->begin();
  CPPUNIT_ASSERT_STRING_EQUAL("Blah4", *(iter++));
  CPPUNIT_ASSERT_EQUAL(false, (bool)iter);

  a->remove("Blah4");
  iter = a->begin();
  CPPUNIT_ASSERT_EQUAL(false, (bool) iter);


  CPPUNIT_ASSERT_EQUAL(true, a->isEmpty());
  /* Now reinsert some data N:testN .. */
  for (int i = 0; i < 5; ++i) {
    char buf[10] = "";
    sprintf(buf, "test%d", i);
    (*a).insert(String(buf));
  }

  int n = 0;
  /* And ensure it comes out in key order in tact */
  for (iter = (*a).begin(); iter; (++iter)) {
    char buf[10] = "";
    sprintf(buf, "test%d", n);
    CPPUNIT_ASSERT_STRING_EQUAL( String(buf), *iter);
    ++n;
  }

  n = (*a).size() - 1;
  /* Test reverse */
  for (iter = (*a).end(); iter; (--iter)) {
    char buf[10] = "";
    sprintf(buf, "test%d", n);
    CPPUNIT_ASSERT_STRING_EQUAL( String(buf), *iter);
    --n;
  }

  /* make sure the reverse did not trample the tree */
  n = 0;
  /* And ensure it comes out in key order in tact */
  for (iter = (*a).begin(); iter; (++iter)) {
    char buf[10] = "";
    sprintf(buf, "test%d", n);
    CPPUNIT_ASSERT_STRING_EQUAL( String(buf), *iter);
    ++n;
  }

  //Remove evens .. check after
  n = 0;
  for (iter = (*a).begin(); iter; (++iter)) {
    //While removing, this next condition should hold true
    if (n % 2) {
      iter.remove();
    }
//   The iterator is INVALID now
//   CPPUNIT_ASSERT_EQUAL(n, iter->key());
    ++n;
  }

  //Ensure odds are still there
  n = 0;
  for (iter = (*a).begin(); iter; (++iter)) {
    char buf[10] = "";
    sprintf(buf, "test%d", n);
    CPPUNIT_ASSERT_STRING_EQUAL( String(buf), *iter);
    n += 2;
  }

  (*a).clear();

  /* Now reinsert some data N:testN .. */
  for (int i = 0; i < 5; ++i) {
    char buf[10] = "";
    sprintf(buf, "test%d", i);
    (*a).insert(String(buf));
  }

  /* Now do the same test in reverse (removing from the end) */
  //Remove evens .. check after
  n = (*a).size() - 1;
  int keep_n = n;
  for (iter = (*a).end(); iter; (--iter)) {
    //While removing, this next condition should hold true
    if (n % 2) {
      iter.remove();
    }
//   The iterator is INVALID now
//   CPPUNIT_ASSERT_EQUAL(n, iter->key());
    --n;
  }

  CPPUNIT_ASSERT_EQUAL(false, a->isEmpty());

  //Ensure odds are still there
  n = keep_n;
  for (iter = (*a).end(); iter; (--iter)) {
    char buf[10] = "";
    sprintf(buf, "test%d", n);
    CPPUNIT_ASSERT_STRING_EQUAL( String(buf), *iter);
    n -= 2;
  }


}
