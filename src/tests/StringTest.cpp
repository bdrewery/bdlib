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

/* StringTest.c
 *
 */
#include <cctype>
#include "StringTest.h"
#include "Array.h"
#include "HashTable.h"
#include <base64.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION (StringTest);

void StringTest :: setUp (void)
{
    strcpy(cstring, "Some static cstring to play with");
    // set up test environment (initializing objects)
    a = new String();
    b = new String("blah");
    c = new String(*b);
    d = new String(cstring);
    e = new String(*d);
    f = new String(cstring, 11);
    g = new String('x');
    h = new String(size_t(35));
}

void StringTest :: tearDown (void)
{
    // finally delete objects
    delete a; delete b; delete c; delete d;
    delete e; delete f; delete g; delete h;
}

void StringTest :: lengthTest (void)
{
  CPPUNIT_ASSERT_EQUAL((size_t) 0, a->length());
  CPPUNIT_ASSERT_EQUAL((size_t) 4, b->length());
  CPPUNIT_ASSERT_EQUAL(b->length(), c->length());
  CPPUNIT_ASSERT_EQUAL(strlen(cstring), d->length());
  CPPUNIT_ASSERT_EQUAL(d->length(), e->length());
  CPPUNIT_ASSERT_EQUAL((size_t) 11, f->length());
  CPPUNIT_ASSERT_EQUAL((size_t) 1, g->length());
  CPPUNIT_ASSERT_EQUAL((size_t) 0, h->length());

  // Check boolean operator
  CPPUNIT_ASSERT_EQUAL((bool) true, *g ? true : false);
}

void StringTest :: clearTest (void)
{
  *b = "test";
  *a = *b;

  CPPUNIT_ASSERT_EQUAL((size_t) 4, b->length());
  b->clear();
  CPPUNIT_ASSERT_EQUAL((size_t) 0, b->length());
  CPPUNIT_ASSERT_EQUAL(true, b->isEmpty());

  *b = "TEST1234";
  CPPUNIT_ASSERT_EQUAL((size_t) 8, b->length());
  CPPUNIT_ASSERT_EQUAL(false, b->isEmpty());

  b->clear();
  CPPUNIT_ASSERT_EQUAL((size_t) 0, b->length());
  CPPUNIT_ASSERT_EQUAL(true, b->isEmpty());

  CPPUNIT_ASSERT_EQUAL((size_t) 4, a->length());
}


void StringTest :: capacityTest (void)
{
  CPPUNIT_ASSERT(b->capacity() >= 4);
  CPPUNIT_ASSERT_EQUAL(b->capacity(), c->capacity());
  CPPUNIT_ASSERT(d->capacity() >= strlen(cstring));
  CPPUNIT_ASSERT_EQUAL(d->capacity(), e->capacity());
  CPPUNIT_ASSERT(f->capacity() >= 11);
  CPPUNIT_ASSERT(g->capacity() >= 1);
  CPPUNIT_ASSERT(h->capacity() >= 35);

  *a = String();
  // This is just a crash test
  CPPUNIT_ASSERT(ssize_t(a->capacity()) >= 0);
}

void StringTest :: compareTest (void)
{
  CPPUNIT_ASSERT_EQUAL(0, (*b).compare("blah"));
  CPPUNIT_ASSERT_EQUAL(0, (*d).compare(cstring));
  CPPUNIT_ASSERT_EQUAL(0, (*f).compare(cstring, 11));
  CPPUNIT_ASSERT((*f).compare(cstring) < 0);
  CPPUNIT_ASSERT((*d).compare(*f) > 0);
  CPPUNIT_ASSERT_EQUAL(0, (*b).compare(*c));
  CPPUNIT_ASSERT(String("abc") < String("def"));
  CPPUNIT_ASSERT(String("abc") < String("abcdef"));
}

void StringTest :: refTest (void)
{
  *a = "Test 1 2 3";
  CPPUNIT_ASSERT_EQUAL(size_t(1), a->rcount());
  *b = *a;
  CPPUNIT_ASSERT_EQUAL(size_t(2), a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(2), b->rcount());
  *c = *b;
  CPPUNIT_ASSERT_EQUAL(size_t(3), a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(3), b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(3), c->rcount());

  *c = "test";
  CPPUNIT_ASSERT_EQUAL(size_t(2), a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(2), b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), c->rcount());

  // Test move semantics
  String my_string(std::move(*a));
  CPPUNIT_ASSERT_EQUAL(size_t(2), my_string.rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(0), a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(2), b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), c->rcount());

  *d = std::move(my_string);
  CPPUNIT_ASSERT_EQUAL(size_t(2), d->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(0), my_string.rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(0), a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(2), b->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), c->rcount());
}

void StringTest :: swapTest (void)
{
  using std::swap;

  *a = "this is a";
  *b = "THIS IS B";

  swap(*a, *b);
  CPPUNIT_ASSERT_STRING_EQUAL ("THIS IS B", *a);
  CPPUNIT_ASSERT_STRING_EQUAL ("this is a", *b);
}

void StringTest :: equalTest (void)
{
  CPPUNIT_ASSERT_EQUAL (*b, *c);
  CPPUNIT_ASSERT_EQUAL (*b, String("blah"));
  CPPUNIT_ASSERT_STRING_EQUAL ("blah", *b);
  CPPUNIT_ASSERT_EQUAL (*d, String(cstring));
  CPPUNIT_ASSERT_STRING_EQUAL (cstring, *d);
  CPPUNIT_ASSERT_EQUAL (*e, String(cstring));
  CPPUNIT_ASSERT_STRING_EQUAL (cstring, *e);
  CPPUNIT_ASSERT_EQUAL (*g, String('x'));
  CPPUNIT_ASSERT_STRING_EQUAL ('x', *g);
  CPPUNIT_ASSERT_EQUAL (*g, String("x"));
  CPPUNIT_ASSERT_STRING_EQUAL ("x", *g);
  CPPUNIT_ASSERT(!(*b != *c));
  CPPUNIT_ASSERT_EQUAL(true, !(*a));
  CPPUNIT_ASSERT_EQUAL(false, !(*b));
}

void StringTest :: setTest (void)
{
  *a = "blah";
  CPPUNIT_ASSERT_STRING_EQUAL ("blah", *a);
  CPPUNIT_ASSERT_EQUAL(*a, *b);
  *h = cstring;
  CPPUNIT_ASSERT_EQUAL(*d, *h);
  *a = "  ";
  CPPUNIT_ASSERT_STRING_EQUAL("  ", *a);
  *b = ' ';
  CPPUNIT_ASSERT_STRING_EQUAL(" ", *b);
  CPPUNIT_ASSERT_STRING_EQUAL("blah", *c);
  *b = *c;
  CPPUNIT_ASSERT_STRING_EQUAL(*c, *b);
  CPPUNIT_ASSERT_STRING_EQUAL("blah", *b);
  *b = *b;
  CPPUNIT_ASSERT_STRING_EQUAL(*c, *b);
  CPPUNIT_ASSERT_STRING_EQUAL("blah", *b);
}

void StringTest :: c_strTest(void)
{
  const char *b_test = b->c_str();
  const char *c_test = c->c_str();
  const char *d_test = d->c_str();
  const char *e_test = e->c_str();
  const char *f_test = f->c_str();

  CPPUNIT_ASSERT_EQUAL(0, strcmp(b_test, "blah"));
  CPPUNIT_ASSERT_EQUAL(0, strcmp(b_test, c_test));
  CPPUNIT_ASSERT_EQUAL(0, strcmp(d_test, cstring));
  CPPUNIT_ASSERT_EQUAL(0, strcmp(d_test, e_test));
  CPPUNIT_ASSERT(strcmp(f_test, d_test) < 0);

  *f = "TESTING 1 2 3 4";
  *a = (*f)(2, 5);
  const char *x = a->dup();
  CPPUNIT_ASSERT_STRING_EQUAL("STING", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("STING", x);
  delete[] x;
}

void StringTest :: hasIndexTest(void)
{
  CPPUNIT_ASSERT_EQUAL(false, a->hasIndex(0));
  CPPUNIT_ASSERT_EQUAL(true, b->hasIndex(3));
  CPPUNIT_ASSERT_EQUAL(false, b->hasIndex(4));
  CPPUNIT_ASSERT_EQUAL(true, f->hasIndex(10));
  CPPUNIT_ASSERT_EQUAL(false, f->hasIndex(11));
}

void StringTest :: charAtTest(void)
{
  CPPUNIT_ASSERT_EQUAL('b', b->charAt(0));
  CPPUNIT_ASSERT_EQUAL('h', b->charAt(3));
  CPPUNIT_ASSERT_EQUAL((char) 0, b->charAt(4));
  CPPUNIT_ASSERT_EQUAL(cstring[5], d->charAt(5));
  CPPUNIT_ASSERT_EQUAL('x', g->charAt(0));
}

void StringTest :: indexTest(void)
{
  CPPUNIT_ASSERT_EQUAL('b', (char) (*b)[0]);
  CPPUNIT_ASSERT_EQUAL('h', (char) (*b)[3]);
  CPPUNIT_ASSERT_EQUAL(cstring[5], (char) (*d)[5]);
  CPPUNIT_ASSERT_EQUAL('x', (char) (*g)[0]);

  (*a) = "This is a test";
  (*b) = (*a);
  (*b)[0] = 't';
  (*b)[1] = 'H';
  (*b)[2] = (char) (*b)[1];
  (*b)[3] = ((const String) *a)[0];
  (*b)[5] = (*a)[6];
  char chr = (*a)[6];

  (*b)[6] = (*b)[7] = 'a';
  CPPUNIT_ASSERT_EQUAL('a', (char) (*b)[6]);
  CPPUNIT_ASSERT_EQUAL('a', (char) (*b)[7]);

  (*b)[6] = (*b)[7] = (*a)[0];

  (*b)[6] = (*b)[6] = (*b)[6];
 
  CPPUNIT_ASSERT_EQUAL('s', chr);

  CPPUNIT_ASSERT_STRING_EQUAL("This is a test", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("tHHT sTTa test", *b);
}

void StringTest :: iteratorTest(void)
{
  (*a) = (*b) = "this is just a TEST";

  for (size_t i = 0; i < (*a).length(); ++i)
    (*a)[i] = toupper((*a)[i]);

  CPPUNIT_ASSERT_STRING_EQUAL("THIS IS JUST A TEST", (*a));  
  CPPUNIT_ASSERT_STRING_EQUAL("this is just a TEST", (*b));

  for (size_t i = 0; i < (*a).length() - 1; ++i)
    (*a)[i] = (*b)[i+1] = tolower((*a)[i]);

  CPPUNIT_ASSERT_STRING_EQUAL("this is just a tesT", (*a));  
  CPPUNIT_ASSERT_STRING_EQUAL("tthis is just a tes", (*b));
}

void StringTest :: stl_iteratorTest(void)
{
  *a = "abcdefghijk";
  *b = "cfghkijabde";

  sort(b->begin(), b->end());
  CPPUNIT_ASSERT_STRING_EQUAL(*a, *b);
}

void StringTest :: stl_reverse_iteratorTest(void)
{
  *a = "kjihgfedcba";
  *b = "cfghkijabde";

  sort(b->rbegin(), b->rend());
  CPPUNIT_ASSERT_STRING_EQUAL(*a, *b);
}

void StringTest :: stl_const_iteratorTest(void)
{
  const String test("cfghkijabde");
  CPPUNIT_ASSERT_STRING_EQUAL("k", String(max_element(test.begin(), test.end()), 1));
}

void StringTest :: appendTest(void)
{
  a->append('a');
  CPPUNIT_ASSERT_EQUAL(0, strcmp(a->c_str(), "a"));
  a->append("bcd");
  CPPUNIT_ASSERT_STRING_EQUAL("abcd", *a);
  a->append("test", 0);
  CPPUNIT_ASSERT_STRING_EQUAL("abcd", *a);
  f->append(&cstring[11], strlen(cstring) - 11);
  CPPUNIT_ASSERT_STRING_EQUAL(*d, *f);
  c->append(*b);
  CPPUNIT_ASSERT_STRING_EQUAL("blahblah", *c);
  c->append(' ');
  c->append(*c);
  CPPUNIT_ASSERT_STRING_EQUAL("blahblah blahblah ", *c);
  c->append("  ");
  c->append(*b);
  CPPUNIT_ASSERT_STRING_EQUAL("blahblah blahblah   blah", *c);

  /* Test binary data! */
  String tmp(size_t(255));
  for (unsigned char C = 0; C < 255; C++)
    tmp.append(C);
  CPPUNIT_ASSERT_EQUAL((size_t)255, tmp.length());

  char *s = (char*) malloc(255);
  for (unsigned char C = 0; C < 255; C++)
    s[(unsigned int) C] = C;
  String tmp2 = String(s, 255);

  CPPUNIT_ASSERT_STRING_EQUAL(tmp2, tmp);
  CPPUNIT_ASSERT_EQUAL(tmp.length(), tmp2.length());
  free(s);

}

void StringTest :: insertTest(void)
{
  a->insert(1, 'a');
  CPPUNIT_ASSERT_EQUAL(false, a->hasIndex(0));
  a->insert(0, 'b');
  CPPUNIT_ASSERT_STRING_EQUAL("b", *a);
  CPPUNIT_ASSERT_EQUAL(false, a->hasIndex(1));
  b->insert(2, "BLAH");
  CPPUNIT_ASSERT_STRING_EQUAL("blBLAHah", *b);
  CPPUNIT_ASSERT_STRING_EQUAL("blah", *c);
  b->insert(3, "TEST", 0);
  CPPUNIT_ASSERT_STRING_EQUAL("blBLAHah", *b);
  CPPUNIT_ASSERT_STRING_EQUAL("blah", *c);
  b->insert(3, "TEST", 1);
  CPPUNIT_ASSERT_STRING_EQUAL("blBTLAHah", *b);
  CPPUNIT_ASSERT_STRING_EQUAL("blah", *c);

  *a = "this is some long string,this is some long string,this is some long string,this is some long string,this is some long string,this is some long string,this is some long string,this is some long string,this is some long string,this is some long string";
  *c = *a;
  *a = c->substring(10);
  *b = "THIS IS SOME" + *a;
  a->insert(0, "THIS IS SOME");
  CPPUNIT_ASSERT_STRING_EQUAL("THIS IS SOMEme long string,this is some long string,this is some long string,this is some long string,this is some long string,this is some long string,this is some long string,this is some long string,this is some long string,this is some long string", *a);

  *a += a->length();
  a->insert(0, "SOME TEST");
  CPPUNIT_ASSERT_STRING_EQUAL("SOME TEST", *a);

}

void StringTest :: replaceTest(void)
{
  a->replace(1, "wtf");
  CPPUNIT_ASSERT_EQUAL(false, a->hasIndex(0));
  b->replace(1, 'x');
  CPPUNIT_ASSERT_STRING_EQUAL("bxah", *b);
  b->replace(0, "haha");
  CPPUNIT_ASSERT_STRING_EQUAL("haha", *b);
  CPPUNIT_ASSERT_STRING_EQUAL("blah", *c);
  g->replace(1, "ml");
  CPPUNIT_ASSERT_STRING_EQUAL("xml", *g);

  *a = "1 2 3";
  *b = a->sub(" ", ", ");
  CPPUNIT_ASSERT_STRING_EQUAL("1 2 3", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("1, 2, 3", *b);

  *a = "1 2 3";
  *b = a->sub(" ", ",");
  CPPUNIT_ASSERT_STRING_EQUAL("1 2 3", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("1,2,3", *b);

  *a = "1 2 3";
  *b = a->sub("a", ",");
  CPPUNIT_ASSERT_STRING_EQUAL("1 2 3", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("1 2 3", *b);

  *a = "a1 2 3";
  *b = a->sub("a", ",");
  CPPUNIT_ASSERT_STRING_EQUAL("a1 2 3", *a);
  CPPUNIT_ASSERT_STRING_EQUAL(",1 2 3", *b);

  *a = "a1 2 3a";
  *b = a->sub("a", ",");
  CPPUNIT_ASSERT_STRING_EQUAL("a1 2 3a", *a);
  CPPUNIT_ASSERT_STRING_EQUAL(",1 2 3,", *b);

  *a = "1 2 3a";
  *b = a->sub("a", ",");
  CPPUNIT_ASSERT_STRING_EQUAL("1 2 3a", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("1 2 3,", *b);

  *a = "1 2 3aa";
  *b = a->sub("a", ",");
  CPPUNIT_ASSERT_STRING_EQUAL("1 2 3aa", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("1 2 3,,", *b);

  *a = "aa1 2 3aa";
  *b = a->sub("a", ",");
  CPPUNIT_ASSERT_STRING_EQUAL("aa1 2 3aa", *a);
  CPPUNIT_ASSERT_STRING_EQUAL(",,1 2 3,,", *b);

  *a = "aa1 2 a 3aa";
  *b = a->sub("a", ",");
  CPPUNIT_ASSERT_STRING_EQUAL("aa1 2 a 3aa", *a);
  CPPUNIT_ASSERT_STRING_EQUAL(",,1 2 , 3,,", *b);

  *a = "aa1 2 a 3aa";
  *b = a->sub("a", ",", 1);
  CPPUNIT_ASSERT_STRING_EQUAL("aa1 2 a 3aa", *a);
  CPPUNIT_ASSERT_STRING_EQUAL(",a1 2 a 3aa", *b);

  *a = "aa1 2 a 3aa";
  *b = a->sub("a", ",", 2);
  CPPUNIT_ASSERT_STRING_EQUAL("aa1 2 a 3aa", *a);
  CPPUNIT_ASSERT_STRING_EQUAL(",,1 2 a 3aa", *b);

  *a = "aa1 2 a 3aa";
  *b = a->sub("a", "AAA");
  CPPUNIT_ASSERT_STRING_EQUAL("aa1 2 a 3aa", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("AAAAAA1 2 AAA 3AAAAAA", *b);

  *a = "aa1 2 a 3aa";
  *b = a->sub("aa", ",");
  CPPUNIT_ASSERT_STRING_EQUAL("aa1 2 a 3aa", *a);
  CPPUNIT_ASSERT_STRING_EQUAL(",1 2 a 3,", *b);

  *a = "aa1 2 aaaa 3aa";
  *b = a->sub("aa", ",");
  CPPUNIT_ASSERT_STRING_EQUAL("aa1 2 aaaa 3aa", *a);
  CPPUNIT_ASSERT_STRING_EQUAL(",1 2 ,, 3,", *b);

  *a = String();
  *b = *a->sub("\"", "\\\"");
  CPPUNIT_ASSERT_STRING_EQUAL("", *a);
  CPPUNIT_ASSERT_STRING_EQUAL(*a, *b);
}

void StringTest :: chompTest(void)
{
  (*a) = "testing123";
  (*a).chomp();
  CPPUNIT_ASSERT_STRING_EQUAL("testing123", (*a));

  (*a) = "testing123\n";
  (*a).chomp();
  CPPUNIT_ASSERT_STRING_EQUAL("testing123", (*a));

  (*a) = "testing123\n\n";
  (*a).chomp();
  CPPUNIT_ASSERT_STRING_EQUAL("testing123\n", (*a));

  (*a) = "testing123\r";
  (*a).chomp();
  CPPUNIT_ASSERT_STRING_EQUAL("testing123", (*a));

  (*a) = "testing123\r";
  (*a).chomp();
  CPPUNIT_ASSERT_STRING_EQUAL("testing123", (*a));

  (*a) = "testing123\r\n";
  (*a).chomp();
  CPPUNIT_ASSERT_STRING_EQUAL("testing123", (*a));

  (*a) = "testing123\n\r\n";
  (*a).chomp();
  CPPUNIT_ASSERT_STRING_EQUAL("testing123\n", (*a));

  (*a) = "testing123\n\r\n\r";
  (*a).chomp();
  CPPUNIT_ASSERT_STRING_EQUAL("testing123\n\r\n", (*a));

  (*a) = "testing123\n\r\n\r\n";
  (*a).chomp();
  CPPUNIT_ASSERT_STRING_EQUAL("testing123\n\r\n", (*a));

  /* Check chaining */
  (*a) = "testing123\n\r\n\r\n";
  CPPUNIT_ASSERT_STRING_EQUAL("testing123\n\r\n", (*a).chomp());

  /* Also check the const version */
  const String cstr("testing123\r\n");
  CPPUNIT_ASSERT_STRING_EQUAL("testing123", cstr.chomp());
  CPPUNIT_ASSERT_EQUAL(size_t(12), cstr.length());
  CPPUNIT_ASSERT_EQUAL(size_t(10), cstr.chomp().length());
}

void StringTest :: trimTest(void)
{
  (*a) = "   testing123   ";
  a->trim();
  CPPUNIT_ASSERT_EQUAL(size_t(10), a->length());
  CPPUNIT_ASSERT_STRING_EQUAL("testing123", (*a).trim());

  (*a) = "   \ttesting123\n   \r";
  CPPUNIT_ASSERT_STRING_EQUAL("testing123", (*a).trim());
  CPPUNIT_ASSERT_EQUAL(size_t(10), a->length());

  (*a) = "   	\ntesting123   \t";
  CPPUNIT_ASSERT_STRING_EQUAL("testing123", (*a).trim());
  CPPUNIT_ASSERT_EQUAL(size_t(10), a->length());

  (*a) = "   \n\n\n\ttesting123\v   ";
  CPPUNIT_ASSERT_STRING_EQUAL("testing123", (*a).trim());
  CPPUNIT_ASSERT_EQUAL(size_t(10), a->length());

  (*a) = "\n\n\n\t  \v   ";
  CPPUNIT_ASSERT_STRING_EQUAL("", (*a).trim());
  CPPUNIT_ASSERT_EQUAL(size_t(0), a->length());

  /* Also check the const version */
  const String cstr("   \t\ftesting123\r\n");
  CPPUNIT_ASSERT_STRING_EQUAL("testing123", cstr.trim());
  CPPUNIT_ASSERT_EQUAL(size_t(17), cstr.length());
  CPPUNIT_ASSERT_EQUAL(size_t(10), cstr.trim().length());
}

void StringTest :: findTest(void)
{
  *a = "This is some long string";
  CPPUNIT_ASSERT_EQUAL((size_t)0, a->find('T'));
  CPPUNIT_ASSERT_EQUAL((size_t)1, a->find('h'));
  CPPUNIT_ASSERT_EQUAL((size_t)2, a->find('i'));
  CPPUNIT_ASSERT_EQUAL((size_t)3, a->find('s'));
  CPPUNIT_ASSERT_EQUAL((size_t)13, a->find('l'));
  CPPUNIT_ASSERT_EQUAL((size_t)13, a->find("long"));
  CPPUNIT_ASSERT_EQUAL((size_t)18, a->find("string"));

  CPPUNIT_ASSERT_EQUAL(String::npos, a->find("z"));
  CPPUNIT_ASSERT_EQUAL(String::npos, a->find("notfound"));
}

void StringTest :: ifindTest(void)
{
  *a = "This is some long string";
  CPPUNIT_ASSERT_EQUAL((size_t)18, a->ifind("stRIng"));
  CPPUNIT_ASSERT_EQUAL(String::npos, a->find("stRIng"));
}

void StringTest :: rifindTest(void)
{
  *a = "This is some long string";
  CPPUNIT_ASSERT_EQUAL((size_t)18, a->rifind("stRIng"));
  CPPUNIT_ASSERT_EQUAL(String::npos, a->rfind("stRIng"));
}
void StringTest :: rfindTest(void)
{

  *a = "Filename.tcl";
  CPPUNIT_ASSERT_EQUAL((size_t)7, a->rfind('e'));//Tests ReferenceCountedArray<char>::rfind
  CPPUNIT_ASSERT_EQUAL((size_t)7, a->rfind("e"));//Tests String::rfind(String)
  CPPUNIT_ASSERT_EQUAL((size_t)8, a->rfind('.'));
  CPPUNIT_ASSERT_EQUAL((size_t)9, a->rfind('t'));
  CPPUNIT_ASSERT_EQUAL((size_t)10, a->rfind('c'));
  CPPUNIT_ASSERT_EQUAL((size_t)11, a->rfind('l'));
  CPPUNIT_ASSERT_EQUAL((size_t)9, a->rfind("tcl"));
  CPPUNIT_ASSERT_EQUAL((size_t)8, a->rfind(".tcl"));
  CPPUNIT_ASSERT_EQUAL((size_t)0, a->rfind("File"));
  CPPUNIT_ASSERT_EQUAL((size_t)0, a->rfind("Filename.tcl"));

  CPPUNIT_ASSERT_EQUAL(String::npos, a->rfind("z"));
  CPPUNIT_ASSERT_EQUAL(String::npos, a->rfind("Filename.tclBLA"));
  CPPUNIT_ASSERT_EQUAL(String::npos, a->rfind("Filename.tck"));

  CPPUNIT_ASSERT_EQUAL((size_t)8, a->rfind(".tcl", a->length() - 4));
  *a = "test.tcl.rb";
  CPPUNIT_ASSERT_EQUAL(String::npos, a->rfind(".tcl", a->length() - 4));
}

void StringTest :: newsplitTest(void)
{
  String sentence = "WORD1 WORD2 WORD3 WORD4 WORD5";
  String w1 = newsplit(sentence);

  CPPUNIT_ASSERT_STRING_EQUAL("WORD2 WORD3 WORD4 WORD5", sentence);
  CPPUNIT_ASSERT_STRING_EQUAL("WORD1", w1);

  String w2 = newsplit(sentence);

  CPPUNIT_ASSERT_STRING_EQUAL("WORD3 WORD4 WORD5", sentence);
  CPPUNIT_ASSERT_STRING_EQUAL("WORD2", w2);

  String w3 = newsplit(sentence);

  CPPUNIT_ASSERT_STRING_EQUAL("WORD4 WORD5", sentence);
  CPPUNIT_ASSERT_STRING_EQUAL("WORD3", w3);

  String w4 = newsplit(sentence);

  CPPUNIT_ASSERT_STRING_EQUAL("WORD5", sentence);
  CPPUNIT_ASSERT_STRING_EQUAL("WORD4", w4);

  String w5 = newsplit(sentence);

  CPPUNIT_ASSERT_STRING_EQUAL("WORD5", w5);
  CPPUNIT_ASSERT_STRING_EQUAL("", sentence);
  CPPUNIT_ASSERT_EQUAL(size_t(0), sentence.length());
  CPPUNIT_ASSERT_EQUAL(size_t(5), w5.length());

  String w6 = newsplit(sentence);
  CPPUNIT_ASSERT_STRING_EQUAL("", w6);
  CPPUNIT_ASSERT_STRING_EQUAL("", sentence);
  CPPUNIT_ASSERT_EQUAL(size_t(0), sentence.length());
  CPPUNIT_ASSERT_EQUAL(size_t(0), w6.length());

  /* Try eliminating runs of whitespace */
  *a = "This  is      a  test  ";
  CPPUNIT_ASSERT_STRING_EQUAL("This", newsplit(*a));
  CPPUNIT_ASSERT_STRING_EQUAL("is      a  test  ", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("is", newsplit(*a));
  CPPUNIT_ASSERT_STRING_EQUAL("a  test  ", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("a", newsplit(*a));
  CPPUNIT_ASSERT_STRING_EQUAL("test  ", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("test", newsplit(*a));
  CPPUNIT_ASSERT_STRING_EQUAL("", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("", newsplit(*a));

}

void StringTest :: concatTest(void)
{
 *a = *b + " " + *b;
 CPPUNIT_ASSERT_STRING_EQUAL("blah blah", *a);
 *a = *b + *a;
 CPPUNIT_ASSERT_STRING_EQUAL("blahblah blah", *a);
 delete a;
 a = new String();
 *a = *b + *a;
 CPPUNIT_ASSERT_STRING_EQUAL("blah", *a);
}

void StringTest :: incDecTest(void)
{
  *a = "test";
  CPPUNIT_ASSERT_STRING_EQUAL("test", *a);
  String old = (*a)++;
  CPPUNIT_ASSERT_STRING_EQUAL("test", old);
  CPPUNIT_ASSERT_STRING_EQUAL("est", *a);
  CPPUNIT_ASSERT_EQUAL((size_t) 3, a->length());
  ++(*a);
  CPPUNIT_ASSERT_STRING_EQUAL("st", *a);
  CPPUNIT_ASSERT_EQUAL((size_t) 2, a->length());
  old = (*a)--;
  CPPUNIT_ASSERT_STRING_EQUAL("st", old);
  CPPUNIT_ASSERT_STRING_EQUAL("s", *a);
  CPPUNIT_ASSERT_EQUAL((size_t) 1, a->length());
  --(*a);
  CPPUNIT_ASSERT_STRING_EQUAL("", *a);
  CPPUNIT_ASSERT_EQUAL((size_t) 0, a->length());
}

void StringTest :: incDecEqualTest(void)
{
  *a = "test word";
  *b = *a;
  *a += size_t(5);
  CPPUNIT_ASSERT_STRING_EQUAL("test word", *b);
  CPPUNIT_ASSERT_EQUAL((size_t) 9, b->length());
  CPPUNIT_ASSERT_STRING_EQUAL("word", *a);
  CPPUNIT_ASSERT_EQUAL((size_t) 4, a->length());


  *b = "blah";
  a->clear();
  *a += "longtest";
  CPPUNIT_ASSERT_EQUAL((size_t) 8, a->length());
  CPPUNIT_ASSERT_EQUAL((size_t) 4, b->length());

  CPPUNIT_ASSERT_STRING_EQUAL("longtest", *a);
  *a -= 3;
  CPPUNIT_ASSERT_EQUAL((size_t) 5, a->length());
  CPPUNIT_ASSERT_STRING_EQUAL("longt", *a);
  *a += size_t(4);
  CPPUNIT_ASSERT_STRING_EQUAL("t", *a);
  CPPUNIT_ASSERT_EQUAL((size_t) 1, a->length());
  CPPUNIT_ASSERT_STRING_EQUAL("blah", *b);
  CPPUNIT_ASSERT_EQUAL((size_t) 4, b->length());

  *a = *a + *b;
  CPPUNIT_ASSERT_STRING_EQUAL("tblah", *a);
  CPPUNIT_ASSERT_EQUAL((size_t) 5, a->length());

  a->Reserve(30); // Avoid COW during these offset tests
  *b = "blah";
  *a = "longtest";
  *a -= 3;
  *a += size_t(4);
  CPPUNIT_ASSERT_STRING_EQUAL("t", *a);
  CPPUNIT_ASSERT_EQUAL((size_t) 1, a->length());

  *a += *b;

  CPPUNIT_ASSERT_STRING_EQUAL("tblah", *a);
  CPPUNIT_ASSERT_EQUAL((size_t) 5, a->length());


  a->replace(0, "test");
  CPPUNIT_ASSERT_STRING_EQUAL("testh", *a);
  CPPUNIT_ASSERT_EQUAL((size_t) 5, a->length());

  a->insert(1, "TEST");
  CPPUNIT_ASSERT_STRING_EQUAL("tTESTesth", *a);
  CPPUNIT_ASSERT_EQUAL((size_t) 9, a->length());

  a->insert(2, String("BLAH"));
  CPPUNIT_ASSERT_STRING_EQUAL("tTBLAHESTesth", *a);
  CPPUNIT_ASSERT_EQUAL((size_t) 13, a->length());

  a->replace(2, String("blah"));
  CPPUNIT_ASSERT_STRING_EQUAL("tTblahESTesth", *a);
  CPPUNIT_ASSERT_EQUAL((size_t) 13, a->length());

  a->replace(2, 'B');
  CPPUNIT_ASSERT_STRING_EQUAL("tTBlahESTesth", *a);
  CPPUNIT_ASSERT_EQUAL((size_t) 13, a->length());

  a->insert(5, 'C');
  CPPUNIT_ASSERT_STRING_EQUAL("tTBlaChESTesth", *a);
  CPPUNIT_ASSERT_EQUAL((size_t) 14, a->length());

  *a = "someword";
  *a += size_t(4);
  CPPUNIT_ASSERT_STRING_EQUAL("word", *a);
  CPPUNIT_ASSERT_EQUAL((size_t) 4, a->length());
  *a += String("test1234") * 20;
  CPPUNIT_ASSERT_STRING_EQUAL("wordtest1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234", *a);
  CPPUNIT_ASSERT_EQUAL((size_t) 4 + (8 * 20), a->length());

  *b = *a;
  *b += size_t(4);
  CPPUNIT_ASSERT_STRING_EQUAL("test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234", *b);
  CPPUNIT_ASSERT_EQUAL((size_t) (8 * 20), b->length());

  a->clear();
  *b += String("test") * 500;
  CPPUNIT_ASSERT_STRING_EQUAL("test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234test1234testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttest", *b);
  CPPUNIT_ASSERT_EQUAL((size_t) ((8 * 20) + (4 * 500)), b->length());

  *a = "test";
  *a += size_t(10);
  CPPUNIT_ASSERT_EQUAL((size_t)0, a->length());
  CPPUNIT_ASSERT_STRING_EQUAL("", *a);

  *a = "test";
  *a -= 10;
  CPPUNIT_ASSERT_EQUAL((size_t)0, a->length());
  CPPUNIT_ASSERT_STRING_EQUAL("", *a);

  *a = "tblah";
  *b = "blah";

  *d += *d + *d;
  char check[100];
  strcpy(check, cstring); // *d = cstring
  strcat(check, cstring); 
  strcat(check, cstring);
  CPPUNIT_ASSERT_STRING_EQUAL(check, *d);

  *b += *a;
  *c = *c + *a;

  CPPUNIT_ASSERT_STRING_EQUAL("blahtblah", *b);
  CPPUNIT_ASSERT_STRING_EQUAL("blahtblah", *c);
  CPPUNIT_ASSERT_STRING_EQUAL(*c, *b);

  *a = "";
  *a += size_t(4);
  *a -= 4;
  CPPUNIT_ASSERT_EQUAL((size_t)0, a->length());

  *a = " \r\n";
  *b = a->substring(0, 1);
  CPPUNIT_ASSERT_EQUAL((size_t)1, b->length());
  CPPUNIT_ASSERT_EQUAL((size_t)3, a->length());
  CPPUNIT_ASSERT_EQUAL(true, strcmp(" ", b->c_str()) == 0);
  CPPUNIT_ASSERT_EQUAL((size_t)1, b->length());
  CPPUNIT_ASSERT_EQUAL((size_t)3, a->length());
  (*a) += size_t(2);
  (*a) += size_t(1);
  CPPUNIT_ASSERT_EQUAL((size_t)1, b->length());
  CPPUNIT_ASSERT_EQUAL((size_t)0, a->length());
  CPPUNIT_ASSERT_EQUAL((size_t)0, strlen(a->c_str()));
}

void StringTest :: operatorStarTest(void)
{
  *a = "Test! ";
  *b = *a * 3;
  CPPUNIT_ASSERT_STRING_EQUAL("Test! Test! Test! ", *b);

  const String mytest("Test! ");
  *b = mytest * 3;
  CPPUNIT_ASSERT_STRING_EQUAL("Test! Test! Test! ", *b);
}

void StringTest :: printfTest(void)
{
  int n = 506;
  const char* s = "something";

  *a = String::printf("%s %d", s, n);
  CPPUNIT_ASSERT_STRING_EQUAL("something 506", *a);

  CPPUNIT_ASSERT_STRING_EQUAL("506 something 506", String::printf("%d %s %d", n, s, n));
}

/*
    a = new String();
    b = new String("blah");
    c = new String(*b);
    d = new String(cstring);
    e = new String(*d);
    f = new String(cstring, 11);
    g = new String('x');
    h = new String(35);
*/

void StringTest :: base64Test(void)
{
  *b = "b";
  *c = *b;
  *a = base64Encode(*b);
  CPPUNIT_ASSERT(*a != *c);
  CPPUNIT_ASSERT(*a != "b");
  CPPUNIT_ASSERT_STRING_EQUAL("Yg==", *a);
  *b = base64Decode(*a);
  CPPUNIT_ASSERT_STRING_EQUAL(*c, *b);
  CPPUNIT_ASSERT_STRING_EQUAL("b", *b);

  *b = "bl";
  *c = *b;
  *a = base64Encode(*b);
  CPPUNIT_ASSERT(*a != *c);
  CPPUNIT_ASSERT(*a != "bl");
  CPPUNIT_ASSERT_STRING_EQUAL("Ymw=", *a);
  *b = base64Decode(*a);
  CPPUNIT_ASSERT_STRING_EQUAL(*c, *b);
  CPPUNIT_ASSERT_STRING_EQUAL("bl", *b);

  *b = "bla";
  *c = *b;
  *a = base64Encode(*b);
  CPPUNIT_ASSERT(*a != *c);
  CPPUNIT_ASSERT(*a != "bla");
  CPPUNIT_ASSERT_STRING_EQUAL("Ymxh", *a);
  *b = base64Decode(*a);
  CPPUNIT_ASSERT_STRING_EQUAL(*c, *b);
  CPPUNIT_ASSERT_STRING_EQUAL("bla", *b);

  *b = "blah";
  *c = *b;
  *a = base64Encode(*b);
  CPPUNIT_ASSERT(*a != *c);
  CPPUNIT_ASSERT(*a != "blah");
  CPPUNIT_ASSERT_STRING_EQUAL("YmxhaA==", *a);
  *b = base64Decode(*a);
  CPPUNIT_ASSERT_STRING_EQUAL(*c, *b);
  CPPUNIT_ASSERT_STRING_EQUAL("blah", *b);



  *d = base64Encode(*d);
  *d = base64Decode(*d);
  CPPUNIT_ASSERT_STRING_EQUAL(cstring, *d);

  String eff = String(*f);
  *f = base64Encode(*f);
  *f = base64Decode(*f);
  CPPUNIT_ASSERT_STRING_EQUAL(*f, eff);

  /* Test misc lengths of alphabet */
  char *buf = (char*) malloc(100);
  for (int i = 0; i < 100; i++) {
    buf[0] = 0;
    /* Create a buffer of length i */
    for (int k = 0; k < i; k++) {
      char letter[2];
      letter[0] = 'a' + k;
      letter[1] = 0;
      strcat(buf, letter);
    }
    buf[i] = 0;
    String tmpbuf(base64Encode(buf));
//printf("%s -> %s\n", buf, tmpbuf.c_str());
    String decoded(base64Decode(tmpbuf));
    CPPUNIT_ASSERT_STRING_EQUAL(buf, decoded);
    CPPUNIT_ASSERT_EQUAL((size_t) i, decoded.length());
  }
  free(buf);

  String tmp;

  const char *twentysix = "this is 26 characters long";
  tmp = twentysix;
  tmp = base64Encode(tmp);
  tmp = base64Decode(tmp);
  CPPUNIT_ASSERT_STRING_EQUAL(twentysix, tmp);
  CPPUNIT_ASSERT_EQUAL(strlen(twentysix), tmp.length());

  const char *twentynine = "this is 29 characters long ok";
  tmp = twentynine;
  tmp = base64Encode(tmp);
  tmp = base64Decode(tmp);
  CPPUNIT_ASSERT_STRING_EQUAL(twentynine, tmp);
  CPPUNIT_ASSERT_EQUAL(strlen(twentynine), tmp.length());

  tmp = "";
  tmp.Reserve(255);
  for (unsigned char C = 0; C < 255; C++)
    tmp.append(C);
  String save = String(tmp);
  tmp = base64Encode(tmp);
  tmp = base64Decode(tmp);
  CPPUNIT_ASSERT_EQUAL((size_t)255, tmp.length());
  CPPUNIT_ASSERT_STRING_EQUAL(save, tmp);
  CPPUNIT_ASSERT_EQUAL(save.length(), tmp.length());


  /* Do heavy stress testing using /dev/urandom */
  int fd = open("/dev/urandom", O_RDONLY);
  char ubuf[1024];
  size_t size = 1;

  for (int i = 0; i < 200; ++i) {
    ++size;
    read(fd, ubuf, size);

    (*a) = String(ubuf, size);
    (*b) = base64Encode(*a);
    (*c) = base64Decode(*b);

    // Ensure they still match
    CPPUNIT_ASSERT_STRING_EQUAL(*b, base64Encode(*c));
    CPPUNIT_ASSERT_STRING_EQUAL(*a, *c);
  }
  close(fd);
}

#ifdef DISABLED
void StringTest :: encryptTest(void)
{
  b->encrypt(nullptr);
  CPPUNIT_ASSERT(*b == *c);
  CPPUNIT_ASSERT(*b == "blah");
  b->encrypt("");
  CPPUNIT_ASSERT(*b == *c);
  CPPUNIT_ASSERT(*b == "blah");

  b->encrypt("doggie");
  CPPUNIT_ASSERT(*b != *c);
  CPPUNIT_ASSERT(*b != "blah");
  String tmp = *b;
  tmp.decrypt("horse");
  CPPUNIT_ASSERT(tmp != *c);
  CPPUNIT_ASSERT(tmp != "blah");
  tmp.encrypt("horse");
  CPPUNIT_ASSERT(tmp == *b);
  tmp.decrypt("doggie");
  CPPUNIT_ASSERT(tmp == "blah");
  CPPUNIT_ASSERT(tmp == *c);
  b->decrypt("doggie");
  CPPUNIT_ASSERT(*b == *c);
  CPPUNIT_ASSERT(*b == "blah");
}

void StringTest :: encryptBase64Test(void)
{
  b->encrypt("doggie");
  b->base64Encode();
  b->base64Decode();
  b->decrypt("doggie");
  CPPUNIT_ASSERT(*b == *c);

  *a = "kGiq0kPEuB7A.3nAGG1D3E..";
  a->decrypt("OeG3yxvQXHq0iw0olaG9mZE8QlmH6vn2");
  a->base64Decode();

  *b = "! uid 1000";
  b->encrypt("OeG3yxvQXHq0iw0olaG9mZE8QlmH6vn2");
  b->base64Encode();
  CPPUNIT_ASSERT(*b == "kGiq0kPEuB7A.3nAGG1D3E..");
  b->base64Decode();
  b->decrypt("OeG3yxvQXHq0iw0olaG9mZE8QlmH6vn2");
  CPPUNIT_ASSERT(*b == "! uid 1000");
}
#endif /* DISABLED */

void StringTest :: substringTest(void)
{
  *a = "Why hello there!";
  CPPUNIT_ASSERT_EQUAL(size_t(1), a->rcount());
  String substring(a->substring(4, 5));
  CPPUNIT_ASSERT_EQUAL(size_t(2), a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(2), substring.rcount());
  CPPUNIT_ASSERT_STRING_EQUAL("Why hello there!", *a);
  CPPUNIT_ASSERT('h' == substring[0]);
  CPPUNIT_ASSERT_STRING_EQUAL("hello", substring);
  substring += " bryan";
  CPPUNIT_ASSERT_EQUAL(size_t(1), a->rcount());
  CPPUNIT_ASSERT_EQUAL(size_t(1), substring.rcount());
  CPPUNIT_ASSERT_STRING_EQUAL("Why hello there!", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("hello bryan", substring);

  *a = "there goes my reference!";
  CPPUNIT_ASSERT_STRING_EQUAL("there goes my reference!", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("hello bryan", substring);

  *a = "there goes my reference!";
  CPPUNIT_ASSERT_STRING_EQUAL("there goes my reference!", *a);
  substring = a->substring(6, 4);
  *a = "now it is really gone";
  CPPUNIT_ASSERT_STRING_EQUAL("goes", substring);

  *a = "0123456789";
  substring = a->substring(5, 5);
  CPPUNIT_ASSERT_STRING_EQUAL("56789", substring);

  /* Check substring of substring */
  String substring2 = substring.substring(0, 5);
  CPPUNIT_ASSERT_STRING_EQUAL("56789", substring);
  CPPUNIT_ASSERT_STRING_EQUAL("56789", substring2);

  /* Check substring of substring of substring */
  substring2 = substring.substring(0, 5);
  String substring3 = substring2.substring(1, 4);
  CPPUNIT_ASSERT_STRING_EQUAL("56789", substring);
  CPPUNIT_ASSERT_STRING_EQUAL("56789", substring2);
  CPPUNIT_ASSERT_STRING_EQUAL("6789", substring3);

  *a = "abcdefgh";
  substring = a->substring(3,-2);
  CPPUNIT_ASSERT_STRING_EQUAL("abcdefgh", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("def", substring);

  substring = a->substring(-3, 2);
  CPPUNIT_ASSERT_STRING_EQUAL("fg", substring);

  substring = a->substring(-1,1);
  CPPUNIT_ASSERT_STRING_EQUAL("h", substring);

  substring = a->substring(-1, 5);
  CPPUNIT_ASSERT_STRING_EQUAL("h", substring);

  substring = a->substring(0, 30);
  CPPUNIT_ASSERT_STRING_EQUAL("abcdefgh", substring);

  *a = "This is a test";
  substring = (*a)(-4,4);
  CPPUNIT_ASSERT_STRING_EQUAL("test", substring);

  substring2 = substring;
  CPPUNIT_ASSERT_STRING_EQUAL("test", substring);
  CPPUNIT_ASSERT_STRING_EQUAL("test", substring2);
  CPPUNIT_ASSERT_STRING_EQUAL("This is a test", *a);

  (*a) = "this is just a TEST";
  substring = (*a)(0,4);

  for (size_t i = 0; i < (*a).length(); ++i)
    (*a)[i] = toupper((*a)[i]);

  CPPUNIT_ASSERT_STRING_EQUAL("THIS IS JUST A TEST", (*a));  
  CPPUNIT_ASSERT_STRING_EQUAL("this", substring);

  (*a) = "this is just a TEST";
  substring = (*a)(0,4);

  for (size_t i = 0; i < substring.length(); ++i)
    substring[i] = toupper(substring[i]);

  CPPUNIT_ASSERT_STRING_EQUAL("this is just a TEST", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("THIS", substring);  

  String replace;

  *b = *a;
  replace = "THIS";
  (*a)(0, 4) = replace;
  CPPUNIT_ASSERT_STRING_EQUAL("THIS is just a TEST", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("THIS", replace);
  CPPUNIT_ASSERT_STRING_EQUAL("this is just a TEST", *b);

  *a = "this is just a TEST";
  *b = *a;
  replace = "THAT";
  (*a)(0, 4) = "THAT";
  CPPUNIT_ASSERT_STRING_EQUAL("THAT is just a TEST", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("THAT", replace);
  CPPUNIT_ASSERT_STRING_EQUAL("this is just a TEST", *b);

  *a = "this is just a TEST";
  *b = *a;
  (*a)(0, 4) = (*b)(15, 4);
  CPPUNIT_ASSERT_STRING_EQUAL("TEST is just a TEST", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("this is just a TEST", *b);

  *a = "this is just a TEST";
  *b = *a;
  (*a)(0, 4) = (*a)(15, 4);
  CPPUNIT_ASSERT_STRING_EQUAL("TEST is just a TEST", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("this is just a TEST", *b);

  /* this is more of a compile check */
  const String constString("THIS CANNOT BE MODIFIED");
  *a = constString(0, 4);

  constString(0, 4) = "blah"; //Returns a new string

  CPPUNIT_ASSERT_STRING_EQUAL("THIS CANNOT BE MODIFIED", constString);
  CPPUNIT_ASSERT_STRING_EQUAL("THIS", *a);

  *a = "tESt TEST2 test3 TEST4";
  *b = a->substring(0, 4);
  *e = b->substring(1, 2);
  *c = a->substring(5, 5);
  *d = a->substring(11, 5);
  CPPUNIT_ASSERT_STRING_EQUAL("tESt TEST2 test3 TEST4", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("tESt", *b);
  CPPUNIT_ASSERT_STRING_EQUAL("ES", *e);
  CPPUNIT_ASSERT_STRING_EQUAL("TEST2", *c);
  CPPUNIT_ASSERT_STRING_EQUAL("test3", *d);
  CPPUNIT_ASSERT_EQUAL(0, strcmp(a->c_str(), "tESt TEST2 test3 TEST4"));
  CPPUNIT_ASSERT_EQUAL(0, strcmp(b->c_str(), "tESt"));
  CPPUNIT_ASSERT_EQUAL(0, strcmp(e->c_str(), "ES"));
  CPPUNIT_ASSERT_EQUAL(0, strcmp(c->c_str(), "TEST2"));
  CPPUNIT_ASSERT_EQUAL(0, strcmp(d->c_str(), "test3"));
  CPPUNIT_ASSERT_STRING_EQUAL("tESt TEST2 test3 TEST4", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("tESt", *b);
  CPPUNIT_ASSERT_STRING_EQUAL("ES", *e);
  CPPUNIT_ASSERT_STRING_EQUAL("TEST2", *c);
  CPPUNIT_ASSERT_STRING_EQUAL("test3", *d);


  *a = "does arbitrary inserting and replacing work with substrings?";
  CPPUNIT_ASSERT_EQUAL(size_t(60), a->length());

  *b = (*a)(5, 9);
  CPPUNIT_ASSERT_STRING_EQUAL("arbitrary", *b);
  CPPUNIT_ASSERT_STRING_EQUAL("does arbitrary inserting and replacing work with substrings?", *a);
  CPPUNIT_ASSERT_EQUAL(size_t(60), a->length());
  CPPUNIT_ASSERT_EQUAL(size_t(9), b->length());

  *c = (*a)(15, 9);
  CPPUNIT_ASSERT_STRING_EQUAL("arbitrary", *b);
  CPPUNIT_ASSERT_STRING_EQUAL("does arbitrary inserting and replacing work with substrings?", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("inserting", *c);
  CPPUNIT_ASSERT_EQUAL(size_t(60), a->length());
  CPPUNIT_ASSERT_EQUAL(size_t(9), b->length());
  CPPUNIT_ASSERT_EQUAL(size_t(9), c->length());

  *d = *a;
  *d += size_t(20);

  CPPUNIT_ASSERT_STRING_EQUAL("ting and replacing work with substrings?", *d);
  CPPUNIT_ASSERT_STRING_EQUAL("arbitrary", *b);
  CPPUNIT_ASSERT_STRING_EQUAL("does arbitrary inserting and replacing work with substrings?", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("inserting", *c);
  CPPUNIT_ASSERT_EQUAL(size_t(60), a->length());
  CPPUNIT_ASSERT_EQUAL(size_t(9), b->length());
  CPPUNIT_ASSERT_EQUAL(size_t(9), c->length());
  CPPUNIT_ASSERT_EQUAL(size_t(40), d->length());

  *b += " words";
  CPPUNIT_ASSERT_STRING_EQUAL("ting and replacing work with substrings?", *d);
  CPPUNIT_ASSERT_STRING_EQUAL("arbitrary words", *b);
  CPPUNIT_ASSERT_STRING_EQUAL("does arbitrary inserting and replacing work with substrings?", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("inserting", *c);
  CPPUNIT_ASSERT_EQUAL(size_t(60), a->length());
  CPPUNIT_ASSERT_EQUAL(size_t(15), b->length());
  CPPUNIT_ASSERT_EQUAL(size_t(9), c->length());
  CPPUNIT_ASSERT_EQUAL(size_t(40), d->length());

  *b += *c;
  CPPUNIT_ASSERT_STRING_EQUAL("ting and replacing work with substrings?", *d);
  CPPUNIT_ASSERT_STRING_EQUAL("arbitrary wordsinserting", *b);
  CPPUNIT_ASSERT_STRING_EQUAL("does arbitrary inserting and replacing work with substrings?", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("inserting", *c);
  CPPUNIT_ASSERT_EQUAL(size_t(60), a->length());
  CPPUNIT_ASSERT_EQUAL(size_t(24), b->length());
  CPPUNIT_ASSERT_EQUAL(size_t(9), c->length());
  CPPUNIT_ASSERT_EQUAL(size_t(40), d->length());

  *d += "test";
  CPPUNIT_ASSERT_EQUAL(0, strcmp(b->c_str(), "arbitrary wordsinserting"));
  CPPUNIT_ASSERT_STRING_EQUAL("ting and replacing work with substrings?test", *d);
  CPPUNIT_ASSERT_STRING_EQUAL("arbitrary wordsinserting", *b);
  CPPUNIT_ASSERT_STRING_EQUAL("does arbitrary inserting and replacing work with substrings?", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("inserting", *c);
  CPPUNIT_ASSERT_EQUAL(size_t(60), a->length());
  CPPUNIT_ASSERT_EQUAL(size_t(24), b->length());
  CPPUNIT_ASSERT_EQUAL(size_t(9), c->length());
  CPPUNIT_ASSERT_EQUAL(size_t(44), d->length());

  *c -= 1;
  *c += size_t(5);
  *c += "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  *c -= 1;
  *c += "test";
  *c += size_t(1);
  CPPUNIT_ASSERT_STRING_EQUAL("ting and replacing work with substrings?test", *d);
  CPPUNIT_ASSERT_STRING_EQUAL("arbitrary wordsinserting", *b);
  CPPUNIT_ASSERT_STRING_EQUAL("does arbitrary inserting and replacing work with substrings?", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("inaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaatest", *c);
  CPPUNIT_ASSERT_EQUAL(size_t(60), a->length());
  CPPUNIT_ASSERT_EQUAL(size_t(24), b->length());
  CPPUNIT_ASSERT_EQUAL(size_t(105), c->length());
  CPPUNIT_ASSERT_EQUAL(size_t(44), d->length());

  *e = (*d)(5);
  CPPUNIT_ASSERT_STRING_EQUAL("and replacing work with substrings?test", *e);
  CPPUNIT_ASSERT_EQUAL(size_t(44 - 5), e->length());
  CPPUNIT_ASSERT_STRING_EQUAL("ting and replacing work with substrings?test", *d);
  CPPUNIT_ASSERT_STRING_EQUAL("arbitrary wordsinserting", *b);
  CPPUNIT_ASSERT_STRING_EQUAL("does arbitrary inserting and replacing work with substrings?", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("inaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaatest", *c);
  CPPUNIT_ASSERT_EQUAL(size_t(60), a->length());
  CPPUNIT_ASSERT_EQUAL(size_t(24), b->length());
  CPPUNIT_ASSERT_EQUAL(size_t(105), c->length());
  CPPUNIT_ASSERT_EQUAL(size_t(44), d->length());

  *e = (*d)(5);
  *f = *e;
  *e += *d * 20;
  CPPUNIT_ASSERT_STRING_EQUAL("and replacing work with substrings?test", *f);
  CPPUNIT_ASSERT_STRING_EQUAL("and replacing work with substrings?testting and replacing work with substrings?testting and replacing work with substrings?testting and replacing work with substrings?testting and replacing work with substrings?testting and replacing work with substrings?testting and replacing work with substrings?testting and replacing work with substrings?testting and replacing work with substrings?testting and replacing work with substrings?testting and replacing work with substrings?testting and replacing work with substrings?testting and replacing work with substrings?testting and replacing work with substrings?testting and replacing work with substrings?testting and replacing work with substrings?testting and replacing work with substrings?testting and replacing work with substrings?testting and replacing work with substrings?testting and replacing work with substrings?testting and replacing work with substrings?test", *e);
  CPPUNIT_ASSERT_EQUAL(size_t((21 * 44) - 5), e->length());
  CPPUNIT_ASSERT(e->capacity() > d->capacity());
  CPPUNIT_ASSERT_STRING_EQUAL("ting and replacing work with substrings?test", *d);
  CPPUNIT_ASSERT_STRING_EQUAL("arbitrary wordsinserting", *b);
  CPPUNIT_ASSERT_STRING_EQUAL("does arbitrary inserting and replacing work with substrings?", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("inaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaatest", *c);
  CPPUNIT_ASSERT_EQUAL(size_t(60), a->length());
  CPPUNIT_ASSERT_EQUAL(size_t(24), b->length());
  CPPUNIT_ASSERT_EQUAL(size_t(105), c->length());
  CPPUNIT_ASSERT_EQUAL(size_t(44), d->length());


  String big("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa,aaaaaaaaaa");
  CPPUNIT_ASSERT_EQUAL(size_t(130), big.capacity());
  CPPUNIT_ASSERT_EQUAL(size_t(130), big.length());

  String sub = big(120, 10);
  // Detach so 'sub' is no longer shared
  big += "blah";
  CPPUNIT_ASSERT_EQUAL(size_t(130), sub.capacity());
  CPPUNIT_ASSERT_EQUAL(size_t(10), sub.length());

  // This will throw an error in valgrind in String::Reserve due to offsetting problems if not correct
  CPPUNIT_ASSERT_EQUAL(0, strcmp(sub.c_str(), "aaaaaaaaaa"));
  CPPUNIT_ASSERT(sub.capacity() >= 11);
  // This is an optimization check, it should be reusing the original buffer which was 130 big
  CPPUNIT_ASSERT_EQUAL(size_t(130), sub.capacity());

  CPPUNIT_ASSERT_EQUAL(size_t(10), sub.length());
  CPPUNIT_ASSERT_STRING_EQUAL("aaaaaaaaaa", sub);

//  (*a)(-4, 4) = "TEST";
//  CPPUNIT_ASSERT_STRING_EQUAL("This is a TEST", *a);


  *a = "a b c d";
  *b = (*a)(0);
  *c = (*a)(2);
  CPPUNIT_ASSERT_STRING_EQUAL("a b c d", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("a b c d", *b);
  CPPUNIT_ASSERT_STRING_EQUAL("b c d", *c);
}

void StringTest :: splitTest(void)
{
  Array<String> list;

  (*a) = "1 2 3 4";
  list = a->split(' ');
  CPPUNIT_ASSERT_EQUAL(size_t(4), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("4", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(3), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("3", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(2), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("2", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(1), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("1", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("1 2 3 4", (*a));

  (*a) = "1 2 3 4";
  list = a->split(' ', 4);
  CPPUNIT_ASSERT_EQUAL(size_t(4), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("4", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(3), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("3", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(2), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("2", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(1), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("1", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("1 2 3 4", (*a));

  (*a) = "1 2 3 4";
  list = a->split(' ', 3);
  CPPUNIT_ASSERT_EQUAL(size_t(3), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("3 4", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(2), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("2", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(1), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("1", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("1 2 3 4", (*a));


  (*a) = "1 2 3 4";
  list = a->split(' ', 2);
  CPPUNIT_ASSERT_EQUAL(size_t(2), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("2 3 4", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(1), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("1", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("1 2 3 4", (*a));

  (*a) = "1 2 3 4";
  list = a->split(' ', 1);
  CPPUNIT_ASSERT_EQUAL(size_t(1), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("1 2 3 4", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("1 2 3 4", (*a));

  (*a) = "  1   2   3   4  ";
  list = a->split(' ');
  CPPUNIT_ASSERT_EQUAL(size_t(4), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("4", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(3), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("3", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(2), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("2", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(1), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("1", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("  1   2   3   4  ", (*a));

  (*a) = "  1   2   3   4  ";
  list = a->split(' ', 2);
  CPPUNIT_ASSERT_EQUAL(size_t(2), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("2   3   4  ", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(1), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("1", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("  1   2   3   4  ", (*a));

  (*a) = "1--2--3--4";
  list = a->split("--");
  CPPUNIT_ASSERT_EQUAL(size_t(4), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("4", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(3), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("3", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(2), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("2", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(1), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("1", list.pop());
  CPPUNIT_ASSERT_EQUAL(size_t(0), list.length());
  CPPUNIT_ASSERT_STRING_EQUAL("1--2--3--4", (*a));

}

void StringTest :: hashTest(void)
{
  (*a) = "test";
  (*b) = "test";
  (*c) = (*b);
  CPPUNIT_ASSERT_EQUAL(a->hash(), b->hash());
  CPPUNIT_ASSERT_EQUAL(a->hash(), c->hash());
  CPPUNIT_ASSERT_EQUAL(b->hash(), c->hash());

  (*a) = "test1";
  (*b) = "test";
  (*c) = (*b);
  CPPUNIT_ASSERT(a->hash() != b->hash());
  CPPUNIT_ASSERT(a->hash() != c->hash());
  CPPUNIT_ASSERT_EQUAL(b->hash(), c->hash());

  (*b) = (*a)(0, 4);
  CPPUNIT_ASSERT(a->hash() != b->hash());
  (*b) = (*a)(0, 5);
  CPPUNIT_ASSERT(a->hash() == b->hash());
}

void StringTest :: resizeTest(void)
{
  (*a) = "test012345";
  (*b) = *a;
  a->resize(4);
  CPPUNIT_ASSERT_STRING_EQUAL("test", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("test012345", *b);
  CPPUNIT_ASSERT_EQUAL(size_t(4), a->length());
  CPPUNIT_ASSERT_EQUAL(size_t(10), b->length());

  *a = *b;
  a->resize(15, 'c');
  CPPUNIT_ASSERT_STRING_EQUAL("test012345ccccc", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("test012345", *b);
  CPPUNIT_ASSERT_EQUAL(size_t(15), a->length());
  CPPUNIT_ASSERT_EQUAL(size_t(10), b->length());

  *a = (*b)(4, 5);
  CPPUNIT_ASSERT_STRING_EQUAL("01234", *a);
  a->resize(3);
  CPPUNIT_ASSERT_STRING_EQUAL("012", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("test012345", *b);
  CPPUNIT_ASSERT_EQUAL(size_t(3), a->length());
  CPPUNIT_ASSERT_EQUAL(size_t(10), b->length());

  *a = (*b)(4, 5);
  CPPUNIT_ASSERT_STRING_EQUAL("01234", *a);
  a->resize(10);
  CPPUNIT_ASSERT_STRING_EQUAL(bd::String("01234\0\0\0\0\0", 10), *a);
  CPPUNIT_ASSERT_STRING_EQUAL("test012345", *b);
  CPPUNIT_ASSERT_EQUAL(size_t(10), a->length());
  CPPUNIT_ASSERT_EQUAL(size_t(10), b->length());

  *a = (*b)(4, 5);
  CPPUNIT_ASSERT_STRING_EQUAL("01234", *a);
  a->resize(10, 'a');
  CPPUNIT_ASSERT_STRING_EQUAL("01234aaaaa", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("test012345", *b);
  CPPUNIT_ASSERT_EQUAL(size_t(10), a->length());
  CPPUNIT_ASSERT_EQUAL(size_t(10), b->length());

  *a = (*b)(4, 5);
  CPPUNIT_ASSERT_STRING_EQUAL("01234", *a);
  a->resize(4);
  a->resize(10, 'q');
  CPPUNIT_ASSERT_STRING_EQUAL("0123qqqqqq", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("test012345", *b);
  CPPUNIT_ASSERT_EQUAL(size_t(10), a->length());
  CPPUNIT_ASSERT_EQUAL(size_t(10), b->length());

}

void StringTest :: substTest(void) {
  *a = "$this $is $a $test";
  HashTable<String, String> hashes;
  hashes["$this"] = "THIS";
  hashes["$is"] = "IS";
  hashes["$a"] = "A";
  hashes["$test"] = "TEST";
  *b = a->subst(hashes);
  CPPUNIT_ASSERT_STRING_EQUAL("$this $is $a $test", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("THIS IS A TEST", *b);

  *a += " $this";
  *b = a->subst(hashes);
  CPPUNIT_ASSERT_STRING_EQUAL("$this $is $a $test $this", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("THIS IS A TEST THIS", *b);

  hashes[" "] = ",";
  *b = a->subst(hashes);
  CPPUNIT_ASSERT_STRING_EQUAL("$this $is $a $test $this", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("THIS,IS,A,TEST,THIS", *b);
}
/* vim: set sts=2 sw=2 ts=8 et: */
