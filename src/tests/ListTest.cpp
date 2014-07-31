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

/* ListTest.c
 *
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

  (*a) << "One";
  CPPUNIT_ASSERT_EQUAL((size_t)6, a->size());
  (*a) << "Two" << "Three";
  CPPUNIT_ASSERT_EQUAL((size_t)8, a->size());
}

void ListTest :: containsTest (void)
{
  a->insert("Blahstart");
  a->insert("Bleck");
  a->insert("Blah");
  a->insert("Blah");
  a->insert("Blahend");
  (*a) << "One" << "Two";

  CPPUNIT_ASSERT_EQUAL((size_t)7, a->size());  
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blahstart"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Bleck"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blah"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Blahend"));
  CPPUNIT_ASSERT_EQUAL(false, a->contains("Blahk"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("One"));
  CPPUNIT_ASSERT_EQUAL(true, a->contains("Two"));
}

void ListTest :: copyTest (void)
{
  a->insert("Blahstart");
  a->insert("Bleck");
  a->insert("Blah");
  a->insert("Blah");
  a->insert("Blahend");

  delete b;
  b = new List<String>(*a);
  CPPUNIT_ASSERT_EQUAL((size_t)5, b->size());  
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blahstart"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Bleck"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blah"));
  CPPUNIT_ASSERT_EQUAL(true, b->contains("Blahend"));
  CPPUNIT_ASSERT_EQUAL(false, b->contains("Blahk"));

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

void ListTest :: findTest(void)
{
  a->insert("Blah1");
  a->insert("Blah2");
  a->insert("Blah3");
  a->insert("Blah4");

  CPPUNIT_ASSERT_STRING_EQUAL("", a->find("WONT FIND THIS"));

  
  CPPUNIT_ASSERT_STRING_EQUAL("Blah1", a->find("Blah1"));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah2", a->find("Blah2"));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah3", a->find("Blah3"));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah4", a->find("Blah4"));

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

#ifdef NO
void ListTest :: iterateTest (void)
{
  a->insert("Blah1");
  a->insert("Blah2");
  a->insert("Blah3");
  a->insert("Blah4");
  a->insert("Blah5");

  List<int, String>::iterator iter = a->begin();
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

void ListTest :: iterateTest (void)
{
  a->insert("Blah1");
  a->insert("Blah2");
  a->insert("Blah3");
  a->insert("Blah4");
  a->insert("Blah5");

  List<String>::iterator iter = a->begin();
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
/* vim: set sts=2 sw=2 ts=8 et: */
