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

/* BinaryTreeTest.c
 *
 */
#include "BinaryTreeTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (BinaryTreeTest);

void BinaryTreeTest :: setUp (void)
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
  a = new BinaryTree<int, String>();
  b = c = d = e = f = g = h = nullptr;
}

void BinaryTreeTest :: tearDown (void)
{
    // finally delete objects
    delete a; delete b; delete c; delete d;
    delete e; delete f; delete g; delete h;
}

void BinaryTreeTest :: insertTest (void)
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

  String ignored = (*a)[53];
  CPPUNIT_ASSERT_EQUAL(true, ignored.isEmpty());
  CPPUNIT_ASSERT_EQUAL((size_t)5, a->size());

  (*a)[6] = "testIe";
  CPPUNIT_ASSERT_EQUAL((size_t)6, a->size());
}

void BinaryTreeTest :: containsTest (void)
{
  a->insert(1, "Blah");
  a->insert(1, "Bleck");
  a->insert(2, "Blah");
  a->insert(8, "Blah");
  a->insert(4, "Blah");
  (*a)[5] = "testIe";

  CPPUNIT_ASSERT_EQUAL(true, a->contains(5));
  CPPUNIT_ASSERT_EQUAL(true, a->contains(1));
  CPPUNIT_ASSERT_EQUAL(false, a->contains(3));
  CPPUNIT_ASSERT_EQUAL(true, a->contains(4));
}

void BinaryTreeTest :: copyTest (void)
{
  a->insert(2, "Blah2");
  a->insert(1, "Blah");
  a->insert(8, "Blah8");
  a->insert(4, "Blah4");
  a->insert(6, "Blah6");
  a->insert(5, "Blah5");
  a->insert(3, "Blah3");
  a->insert(10, "Blah10");
  

  delete b;
  b = new BinaryTree<int, String>(*a);
  CPPUNIT_ASSERT_EQUAL(true, b->contains(1));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(2));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(3));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(4));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(5));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(6));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(8));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(10));
  CPPUNIT_ASSERT_EQUAL((size_t)8, b->size());

  (*b) = (*a);
  CPPUNIT_ASSERT_EQUAL(true, b->contains(1));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(2));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(3));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(4));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(5));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(6));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(8));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(10));
  CPPUNIT_ASSERT_EQUAL((size_t)8, b->size());

  (*b) = (*b);
  CPPUNIT_ASSERT_EQUAL(true, b->contains(1));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(2));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(3));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(4));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(5));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(6));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(8));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(10));
  CPPUNIT_ASSERT_EQUAL((size_t)8, b->size());
}

void BinaryTreeTest :: clearTest (void) 
{
  a->insert(2, "Blah2");
  a->insert(1, "Blah");
  a->insert(8, "Blah8");
  a->insert(4, "Blah4");
  a->insert(6, "Blah6");
  a->insert(5, "Blah5");
  a->insert(3, "Blah3");
  a->insert(10, "Blah10");

  CPPUNIT_ASSERT_EQUAL((size_t)8, a->size());

  a->clear();

  CPPUNIT_ASSERT_EQUAL((size_t)0, a->size());
  CPPUNIT_ASSERT_EQUAL(true, a->isEmpty());
}

void BinaryTreeTest :: getValueTest (void)
{
  String result;

  a->insert(1, "Blah");
  a->insert(2, "Blah2");
  a->insert(8, "Blah8");
  a->insert(4, "Blah4");

  result = a->getValue(58);
  CPPUNIT_ASSERT_EQUAL(true, result.isEmpty());

  result = a->getValue(1);
  CPPUNIT_ASSERT_STRING_EQUAL("Blah", result);

  a->insert(1, "Bleck");
  
  result = a->getValue(1);
  CPPUNIT_ASSERT_STRING_EQUAL("Bleck", result);

  result = a->getValue(4);
  CPPUNIT_ASSERT_STRING_EQUAL("Blah4", result);

  result = a->getValue(8);
  CPPUNIT_ASSERT_STRING_EQUAL("Blah8", result);

  result = (*a)[8];
  CPPUNIT_ASSERT_STRING_EQUAL("Blah8", result);

  result = (*a)[9];
  CPPUNIT_ASSERT_EQUAL(true, result.isEmpty());

  (*a)[9] = (*a)[8];
  result = (*a)[9];
  CPPUNIT_ASSERT_STRING_EQUAL("Blah8", result);
}

#ifdef DISABLED
void BinaryTreeTest :: getKeyTest (void)
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

void BinaryTreeTest :: removeTest (void)
{
  a->insert(2, "Blah2");
  a->insert(1, "Blah");
  a->insert(8, "Blah8");
  a->insert(4, "Blah4");
  a->insert(6, "Blah6");
  a->insert(5, "Blah5");
  a->insert(3, "Blah3");
  a->insert(10, "Blah10");

  a->remove(8); /* has 2 children */

  String result;

  result = a->getValue(58);
  CPPUNIT_ASSERT_EQUAL(true, result.isEmpty());

 
  CPPUNIT_ASSERT_STRING_EQUAL("Blah", a->getValue(1));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah4", a->getValue(4));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah2", a->getValue(2));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah3", a->getValue(3));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah10", a->getValue(10));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah5", a->getValue(5));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah6", a->getValue(6));
  result = a->getValue(8);
  CPPUNIT_ASSERT_EQUAL(true, result.isEmpty());

  CPPUNIT_ASSERT_EQUAL(false, a->remove(8));

  a->remove(4);
  CPPUNIT_ASSERT_STRING_EQUAL("Blah", a->getValue(1));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah2", a->getValue(2));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah3", a->getValue(3));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah10", a->getValue(10));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah5", a->getValue(5));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah6", a->getValue(6));
  result = a->getValue(4);
  CPPUNIT_ASSERT_EQUAL(true, result.isEmpty());

  a->remove(2);
  CPPUNIT_ASSERT_STRING_EQUAL("Blah", a->getValue(1));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah3", a->getValue(3));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah5", a->getValue(5));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah10", a->getValue(10));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah6", a->getValue(6));
  result = a->getValue(2);
  CPPUNIT_ASSERT_EQUAL(true, result.isEmpty());

  a->remove(3);
  CPPUNIT_ASSERT_STRING_EQUAL("Blah5", a->getValue(5));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah", a->getValue(1));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah10", a->getValue(10));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah6", a->getValue(6));
  result = a->getValue(3);
  CPPUNIT_ASSERT_EQUAL(true, result.isEmpty());

  a->remove(10);
  CPPUNIT_ASSERT_STRING_EQUAL("Blah5", a->getValue(5));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah", a->getValue(1));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah6", a->getValue(6));
  result = a->getValue(10);
  CPPUNIT_ASSERT_EQUAL(true, result.isEmpty());

  a->remove(1);
  CPPUNIT_ASSERT_STRING_EQUAL("Blah5", a->getValue(5));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah6", a->getValue(6));
  result = a->getValue(1);
  CPPUNIT_ASSERT_EQUAL(true, result.isEmpty());

  a->remove(6);
  CPPUNIT_ASSERT_STRING_EQUAL("Blah5", a->getValue(5));
  result = a->getValue(6);
  CPPUNIT_ASSERT_EQUAL(true, result.isEmpty());

  a->remove(5);
  CPPUNIT_ASSERT_EQUAL(true, a->isEmpty());
  result = a->getValue(1);
  CPPUNIT_ASSERT_EQUAL(true, result.isEmpty());
}

void BinaryTreeTest :: iterateTest (void)
{
  a->insert(1, "Blah");
  a->insert(1, "Bleck");
  a->insert(2, "Blah2");
  a->insert(8, "Blah8");
  a->insert(4, "Blah4");
  CPPUNIT_ASSERT_EQUAL(size_t(4), a->size());

  BinaryTree<int, String>::iterator iter = a->begin();

  CPPUNIT_ASSERT_EQUAL(true, (bool) iter);
  while (iter) {
//    int key = (int) iter.next();
//    printf("%d\n", key);
    String value = (iter++)->value();
//    printf("%d: %s\n", key, a->getValue(key));
  }

  iter = a->begin();
  CPPUNIT_ASSERT_EQUAL(true, (bool) iter);
  CPPUNIT_ASSERT_STRING_EQUAL("Bleck", (iter++)->value());
  CPPUNIT_ASSERT_STRING_EQUAL("Blah2", (iter++)->value());
  CPPUNIT_ASSERT_STRING_EQUAL("Blah4", (iter++)->value());
  CPPUNIT_ASSERT_STRING_EQUAL("Blah8", (iter++)->value());
  CPPUNIT_ASSERT_EQUAL(size_t(4), a->size());

  a->remove(4);
  iter = a->begin();
  CPPUNIT_ASSERT_STRING_EQUAL("Bleck", (iter++)->value());
  CPPUNIT_ASSERT_STRING_EQUAL("Blah2", (iter++)->value());
  CPPUNIT_ASSERT_STRING_EQUAL("Blah8", (iter++)->value());
  CPPUNIT_ASSERT_EQUAL(size_t(3), a->size());

  a->remove(2);
  iter = a->begin();
  CPPUNIT_ASSERT_STRING_EQUAL("Bleck", (iter++)->value());
  CPPUNIT_ASSERT_STRING_EQUAL("Blah8", (iter++)->value());
  CPPUNIT_ASSERT_EQUAL(size_t(2), a->size());

  a->remove(8);
  iter = a->begin();
  CPPUNIT_ASSERT_STRING_EQUAL("Bleck", (iter++)->value());
  CPPUNIT_ASSERT_EQUAL(size_t(1), a->size());

  a->remove(1);
  iter = a->begin();
  CPPUNIT_ASSERT_EQUAL(size_t(0), a->size());
  CPPUNIT_ASSERT_EQUAL(false, (bool) iter);

  /* Now reinsert some data N:testN .. */
  for (int i = 0; i < 5; ++i) {
    char buf[10] = "";
    sprintf(buf, "test%d", i);
    (*a).insert(i, String(buf));
  }

  int n = 0;
  /* And ensure it comes out in key order in tact */
  CPPUNIT_ASSERT_EQUAL(true, (bool) (*a).begin());
  for (iter = (*a).begin(); iter; (++iter)) {
    char buf[10] = "";
    sprintf(buf, "test%d", n);
    CPPUNIT_ASSERT_EQUAL(n, iter->key());
    CPPUNIT_ASSERT_STRING_EQUAL( String(buf), iter->value());
    ++n;
  }

  n = (*a).size() - 1;
  /* Test reverse */
  for (iter = (*a).end(); iter; (--iter)) {
    char buf[10] = "";
    sprintf(buf, "test%d", n);
    CPPUNIT_ASSERT_EQUAL(n, iter->key());
    CPPUNIT_ASSERT_STRING_EQUAL( String(buf), iter->value());
    --n;
  }

  /* make sure the reverse did not trample the tree */
  n = 0;
  /* And ensure it comes out in key order in tact */
  for (iter = (*a).begin(); iter; (++iter)) {
    char buf[10] = "";
    sprintf(buf, "test%d", n);
    CPPUNIT_ASSERT_EQUAL(n, iter->key());
    CPPUNIT_ASSERT_STRING_EQUAL( String(buf), iter->value());
    ++n;
  }

  //Remove evens .. check after
  n = 0;
  for (iter = (*a).begin(); iter; (++iter)) {
    //While removing, this next condition should hold true
    CPPUNIT_ASSERT_EQUAL(n, iter->key());
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
    CPPUNIT_ASSERT_EQUAL(n, iter->key());
    CPPUNIT_ASSERT_STRING_EQUAL( String(buf), iter->value());
    n += 2;
  }

  (*a).clear();

  /* Now reinsert some data N:testN .. */
  for (int i = 0; i < 5; ++i) {
    char buf[10] = "";
    sprintf(buf, "test%d", i);
    (*a).insert(i, String(buf));
  }

  /* Now do the same test in reverse (removing from the end) */
  //Remove evens .. check after
  n = (*a).size() - 1;
  int keep_n = n;
  for (iter = (*a).end(); iter; (--iter)) {
    //While removing, this next condition should hold true
    CPPUNIT_ASSERT_EQUAL(n, iter->key());
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
    CPPUNIT_ASSERT_EQUAL(n, iter->key());
    CPPUNIT_ASSERT_STRING_EQUAL( String(buf), iter->value());
    n -= 2;
  }


}
/* vim: set sts=2 sw=2 ts=8 et: */
