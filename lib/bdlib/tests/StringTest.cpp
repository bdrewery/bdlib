/* StringTest.c
 *
 * $Id$
 */
#include "StringTest.h"

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
    h = new String(35);
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
}

void StringTest :: compareTest (void)
{
  CPPUNIT_ASSERT_EQUAL(0, (*b).compare("blah"));
  CPPUNIT_ASSERT_EQUAL(0, (*d).compare(cstring));
  CPPUNIT_ASSERT_EQUAL(0, (*f).compare(cstring, 11));
  CPPUNIT_ASSERT((*f).compare(cstring) < 0);
  CPPUNIT_ASSERT((*d).compare(*f) > 0);
  CPPUNIT_ASSERT_EQUAL(0, (*b).compare(*c));
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
 
  CPPUNIT_ASSERT_EQUAL('s', chr);

  CPPUNIT_ASSERT_STRING_EQUAL("This is a test", *a);
  CPPUNIT_ASSERT_STRING_EQUAL("tHHT sTTa test", *b);
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
}

void StringTest :: replaceTest(void)
{
  a->replace(1, "wtf");
  CPPUNIT_ASSERT_EQUAL(false, a->hasIndex(0));
  b->replace(0, "haha");
  CPPUNIT_ASSERT_STRING_EQUAL("haha", *b);
  CPPUNIT_ASSERT_STRING_EQUAL("blah", *c);
  g->replace(1, "ml");
  CPPUNIT_ASSERT_STRING_EQUAL("xml", *g);
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
  *a += "longtest";
  CPPUNIT_ASSERT_STRING_EQUAL("longtest", *a);
  *a -= 3;
  CPPUNIT_ASSERT_STRING_EQUAL("longt", *a);
  *a += 4;
  CPPUNIT_ASSERT_STRING_EQUAL("t", *a);
  CPPUNIT_ASSERT_EQUAL((size_t) 1, a->length());
  *a += *b;
  CPPUNIT_ASSERT_STRING_EQUAL("tblah", *a);
  CPPUNIT_ASSERT_EQUAL((size_t) 5, a->length());

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
}

void StringTest :: printfTest(void)
{
  int n = 506;
  const char* s = "something";

  a->printf("%s %d", s, n);
  CPPUNIT_ASSERT_STRING_EQUAL("something 506", *a);
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
  b->base64Encode();
  CPPUNIT_ASSERT(*b != *c);
  CPPUNIT_ASSERT(*b != "b");
  CPPUNIT_ASSERT(*b == "Yg==");
  b->base64Decode();
  CPPUNIT_ASSERT(*b == *c);
  CPPUNIT_ASSERT(*b == "b");

  *b = "bl";
  *c = *b;
  b->base64Encode();
  CPPUNIT_ASSERT(*b != *c);
  CPPUNIT_ASSERT(*b != "bl");
  CPPUNIT_ASSERT(*b == "Ymw=");
  b->base64Decode();
  CPPUNIT_ASSERT(*b == *c);
  CPPUNIT_ASSERT(*b == "bl");

  *b = "bla";
  *c = *b;
  b->base64Encode();
  CPPUNIT_ASSERT(*b != *c);
  CPPUNIT_ASSERT(*b != "bla");
  CPPUNIT_ASSERT(*b == "Ymxh");
  b->base64Decode();
  CPPUNIT_ASSERT(*b == *c);
  CPPUNIT_ASSERT(*b == "bla");

  *b = "blah";
  *c = *b;
  b->base64Encode();
  CPPUNIT_ASSERT(*b != *c);
  CPPUNIT_ASSERT(*b != "blah");
  CPPUNIT_ASSERT(*b == "YmxhaA==");
  b->base64Decode();
  CPPUNIT_ASSERT(*b == *c);
  CPPUNIT_ASSERT(*b == "blah");



  d->base64Encode();
  d->base64Decode();
  CPPUNIT_ASSERT(*d == cstring);

  String eff = String(*f);
  f->base64Encode();
  f->base64Decode();
  CPPUNIT_ASSERT(eff == *f);

  const char *cs = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

  String tmp = String(cs);
  tmp.base64Encode();
  tmp.base64Decode();
  CPPUNIT_ASSERT(tmp == cs);
}

#ifdef DISABLED
void StringTest :: encryptTest(void)
{
  b->encrypt(NULL);
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
