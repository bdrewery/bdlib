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

/* HashTableTest.c
 *
 */
#include <unordered_map>
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
  b = new HashTable<int, String>();
  sa = new HashTable<String, String>();
  sb = new HashTable<String, String>();
}

void HashTableTest :: tearDown (void)
{
    // finally delete objects
    delete a;
    delete b;
    delete sa;
    delete sb;
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

  b->insert(27, "Test");
  CPPUNIT_ASSERT_EQUAL((size_t)1, b->size());

  (*b) = (*a);
  CPPUNIT_ASSERT_EQUAL((size_t)4, a->size());
  CPPUNIT_ASSERT_EQUAL((size_t)4, b->size());

  (*a)[42] = "42";
  CPPUNIT_ASSERT_EQUAL((size_t)5, a->size());


  CPPUNIT_ASSERT_EQUAL((size_t)0, sa->size());
  sa->insert("1", "Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)1, sa->size());
  sa->insert("1", "Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)1, sa->size());
  sa->insert("2", "Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)2, sa->size());
  sa->insert("8", "Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)3, sa->size());

  String four("4");
  sa->insert(four, "Blah");
  CPPUNIT_ASSERT_EQUAL((size_t)4, sa->size());
  CPPUNIT_ASSERT_STRING_EQUAL("4", four);
  sa->remove(four);
  CPPUNIT_ASSERT_EQUAL((size_t)3, sa->size());
  String Blah("Blah");

  sa->insert(std::move(four), std::move(Blah));
  CPPUNIT_ASSERT_EQUAL((size_t)4, sa->size());
  CPPUNIT_ASSERT_STRING_EQUAL("", Blah);
  CPPUNIT_ASSERT_STRING_EQUAL("", four);
  CPPUNIT_ASSERT_STRING_EQUAL("Blah", (*sa)["4"]);

  sa->remove("4");
  four = "4";
  Blah = "Blah";
  (*sa)[std::move(four)] = std::move(Blah);
  CPPUNIT_ASSERT_EQUAL((size_t)4, sa->size());
  CPPUNIT_ASSERT_STRING_EQUAL("", Blah);
  CPPUNIT_ASSERT_STRING_EQUAL("", four);
  CPPUNIT_ASSERT_STRING_EQUAL("Blah", (*sa)["4"]);

  sb->insert("27", "Test");
  CPPUNIT_ASSERT_EQUAL((size_t)1, sb->size());

  (*sb) = (*sa);
  CPPUNIT_ASSERT_EQUAL((size_t)4, sa->size());
  CPPUNIT_ASSERT_EQUAL((size_t)4, sb->size());

  (*sa)["hmmzz"] = "hmmm";
  CPPUNIT_ASSERT_EQUAL((size_t)5, sa->size());

  (*sa)["7"] = "seven";
  CPPUNIT_ASSERT_EQUAL((size_t)6, sa->size());
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

  (*b) = (*a);

  CPPUNIT_ASSERT_EQUAL(true, b->contains(1));
  CPPUNIT_ASSERT_EQUAL(false, b->contains(3));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(4));

  b->insert(5, "test");
  CPPUNIT_ASSERT_EQUAL(true, b->contains(5));
  CPPUNIT_ASSERT_EQUAL(false, a->contains(5));

  (*b) = (*a);
  CPPUNIT_ASSERT_EQUAL(true, b->contains(1));
  CPPUNIT_ASSERT_EQUAL(false, b->contains(3));
  CPPUNIT_ASSERT_EQUAL(true, b->contains(4));
  CPPUNIT_ASSERT_EQUAL(false, b->contains(5));
  CPPUNIT_ASSERT_EQUAL(false, a->contains(5));

  (*a)[42] = "42";
  CPPUNIT_ASSERT_EQUAL((size_t)5, a->size());
  CPPUNIT_ASSERT_EQUAL(true, a->contains(42));


  sa->insert("1", "Blah");
  sa->insert("1", "Bleck");
  sa->insert("2", "Blah");
  sa->insert("8", "Blah");
  sa->insert("4", "Blah");
  
  CPPUNIT_ASSERT_EQUAL(true, sa->contains("1"));
  CPPUNIT_ASSERT_EQUAL(false, sa->contains("3"));
  CPPUNIT_ASSERT_EQUAL(true, sa->contains("4"));

  (*sb) = (*sa);

  CPPUNIT_ASSERT_EQUAL(true, sb->contains("1"));
  CPPUNIT_ASSERT_EQUAL(false, sb->contains("3"));
  CPPUNIT_ASSERT_EQUAL(true, sb->contains("4"));

  sb->insert("5", "test");
  CPPUNIT_ASSERT_EQUAL(true, sb->contains("5"));
  CPPUNIT_ASSERT_EQUAL(false, sa->contains("5"));

  (*sb) = (*sa);
  CPPUNIT_ASSERT_EQUAL(true, sb->contains("1"));
  CPPUNIT_ASSERT_EQUAL(false, sb->contains("3"));
  CPPUNIT_ASSERT_EQUAL(true, sb->contains("4"));
  CPPUNIT_ASSERT_EQUAL(false, sb->contains("5"));
  CPPUNIT_ASSERT_EQUAL(false, sa->contains("5"));

  (*sa)["42"] = "42";
  CPPUNIT_ASSERT_EQUAL((size_t)5, sa->size());
  CPPUNIT_ASSERT_EQUAL(true, sa->contains("42"));
}

void HashTableTest :: clearTest (void)
{
    a->insert(1, "Blah");
    a->insert(1, "Bleck");
    a->insert(2, "Blah");
    a->insert(8, "Blah");
    a->insert(4, "Blah");

    CPPUNIT_ASSERT_EQUAL((size_t)4, (*a).size());

    a->clear();

    CPPUNIT_ASSERT_EQUAL((size_t)0, (*a).size());
    CPPUNIT_ASSERT_EQUAL(true, a->isEmpty());

    a->insert(1, "Blah1");
    a->insert(2, "Blah2");
    a->insert(3, "Blah3");

    CPPUNIT_ASSERT_EQUAL((size_t)3, (*a).size());
    CPPUNIT_ASSERT_EQUAL(true, a->contains(1));
    CPPUNIT_ASSERT_EQUAL(true, a->contains(2));
    CPPUNIT_ASSERT_EQUAL(true, a->contains(3));


    sa->insert("1", "Blah");
    sa->insert("1", "Bleck");
    sa->insert("2", "Blah");
    sa->insert("8", "Blah");
    sa->insert("4", "Blah");

    CPPUNIT_ASSERT_EQUAL((size_t)4, (*sa).size());

    sa->clear();

    CPPUNIT_ASSERT_EQUAL((size_t)0, (*sa).size());
    CPPUNIT_ASSERT_EQUAL(true, sa->isEmpty());

    sa->insert("1", "Blah1");
    sa->insert("2", "Blah2");
    sa->insert("3", "Blah3");

    CPPUNIT_ASSERT_EQUAL((size_t)3, (*sa).size());
    CPPUNIT_ASSERT_EQUAL(true, sa->contains("1"));
    CPPUNIT_ASSERT_EQUAL(true, sa->contains("2"));
    CPPUNIT_ASSERT_EQUAL(true, sa->contains("3"));
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
  CPPUNIT_ASSERT_STRING_EQUAL("Blah4", (*a)[4]);
  CPPUNIT_ASSERT_STRING_EQUAL("Blah1", (*a)[1]);
  CPPUNIT_ASSERT_STRING_EQUAL("Blah2", (*a)[2]);
  CPPUNIT_ASSERT_STRING_EQUAL("Blah8", a->getValue(8));
  CPPUNIT_ASSERT_EQUAL((size_t)4, a->size());

  
  String result = (*a)[58];
  CPPUNIT_ASSERT_EQUAL(true, result.isEmpty());
  CPPUNIT_ASSERT_EQUAL((size_t)5, a->size());

  (*b) = (*a);
  CPPUNIT_ASSERT_STRING_EQUAL("Blah4", b->getValue(4));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah1", b->getValue(1));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah2", b->getValue(2));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah8", b->getValue(8));
  CPPUNIT_ASSERT_EQUAL((size_t)5, b->size());
  CPPUNIT_ASSERT_EQUAL((size_t)5, a->size());

  HashTable<int, String> test(*a);
  CPPUNIT_ASSERT_STRING_EQUAL("Blah4", test.getValue(4));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah1", test.getValue(1));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah2", test.getValue(2));
  CPPUNIT_ASSERT_STRING_EQUAL("Blah8", test.getValue(8));
  CPPUNIT_ASSERT_EQUAL((size_t)5, test.size());
  CPPUNIT_ASSERT_EQUAL((size_t)5, a->size());

  (*a)[42] = "42";
  CPPUNIT_ASSERT_STRING_EQUAL("42", (*a)[42]);

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

void HashTableTest :: initializerTest(void) {
  HashTable<String, int> my_hash {
    {"One", 1}, {"Two", 2}
  };
  CPPUNIT_ASSERT_EQUAL(size_t(2), my_hash.size());
  CPPUNIT_ASSERT_EQUAL(1, static_cast<int>(my_hash["One"]));
  CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(my_hash["Two"]));
}

void HashTableTest :: eachTest(void) {
  int x = 5;
  int n;

  a->insert(1, "one");
  a->insert(2, "two");
  n = a->each([](int key, String value, void* param) {
    int _x = *(int*)param;
    if (key == 1)
      CPPUNIT_ASSERT_STRING_EQUAL("one", value);
    else if (key == 2)
      CPPUNIT_ASSERT_STRING_EQUAL("two", value);
    CPPUNIT_ASSERT_EQUAL(5, _x);
  }, &x);

  CPPUNIT_ASSERT_EQUAL(2, n);
}

void HashTableTest :: iteratorTest (void)
{
  a->insert(1, "Blah");
  a->insert(2, "Blah2");
  a->insert(8, "Blah8");
  a->insert(4, "Blah4");

  int i = 0;
  for (auto& kv : (*a)) {
    switch (kv.first) {
      case 1:
        CPPUNIT_ASSERT_STRING_EQUAL("Blah", kv.second);
        break;
      case 2:
        CPPUNIT_ASSERT_STRING_EQUAL("Blah2", kv.second);
        break;
      case 8:
        CPPUNIT_ASSERT_STRING_EQUAL("Blah8", kv.second);
        break;
      case 4:
        CPPUNIT_ASSERT_STRING_EQUAL("Blah4", kv.second);
        break;
    }
    /* Mutate the key for the next test */
    ++kv.second;
    CPPUNIT_ASSERT(i < 4);
    ++i;
  }
  CPPUNIT_ASSERT_EQUAL(4, i);

  /* Check that the key was mutated in the first pass. */
  i = 0;
  for (const auto& kv : (*a)) {
    switch (kv.first) {
      case 1:
        CPPUNIT_ASSERT_STRING_EQUAL("lah", kv.second);
        break;
      case 2:
        CPPUNIT_ASSERT_STRING_EQUAL("lah2", kv.second);
        break;
      case 8:
        CPPUNIT_ASSERT_STRING_EQUAL("lah8", kv.second);
        break;
      case 4:
        CPPUNIT_ASSERT_STRING_EQUAL("lah4", kv.second);
        break;
    }
    CPPUNIT_ASSERT(i < 4);
    ++i;
  }
  CPPUNIT_ASSERT_EQUAL(4, i);
}

/* vim: set sts=2 sw=2 ts=8 et: */
