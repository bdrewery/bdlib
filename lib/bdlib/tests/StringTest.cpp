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
  CPPUNIT_ASSERT (*b == "blah");
  CPPUNIT_ASSERT_EQUAL (*d, String(cstring));
  CPPUNIT_ASSERT (*d == cstring);
  CPPUNIT_ASSERT_EQUAL (*e, String(cstring));
  CPPUNIT_ASSERT (*e == cstring);
  CPPUNIT_ASSERT_EQUAL (*g, String('x'));
  CPPUNIT_ASSERT (*g == 'x');
  CPPUNIT_ASSERT_EQUAL (*g, String("x"));
  CPPUNIT_ASSERT (*g == "x");
  CPPUNIT_ASSERT(!(*b != *c));
  CPPUNIT_ASSERT_EQUAL(true, !(*a));
  CPPUNIT_ASSERT_EQUAL(false, !(*b));
}

void StringTest :: setTest (void)
{
  *a = "blah";
  CPPUNIT_ASSERT (*a == "blah");
  CPPUNIT_ASSERT_EQUAL(*a, *b);
  *h = cstring;
  CPPUNIT_ASSERT_EQUAL(*d, *h);
  *a = "  ";
  CPPUNIT_ASSERT(*a == "  ");
  *b = ' ';
  CPPUNIT_ASSERT(*b == " ");
  CPPUNIT_ASSERT(*c == "blah");
  *b = *c;
  CPPUNIT_ASSERT(*b == *c);
  CPPUNIT_ASSERT(*b == "blah");
  *b = *b;
  CPPUNIT_ASSERT(*b == *c);
  CPPUNIT_ASSERT(*b == "blah");
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
  CPPUNIT_ASSERT_EQUAL('b', (*b)[0]);
  CPPUNIT_ASSERT_EQUAL('h', (*b)[3]);
  CPPUNIT_ASSERT_EQUAL(cstring[5], (*d)[5]);
  CPPUNIT_ASSERT_EQUAL('x', (*g)[0]);
}

void StringTest :: appendTest(void)
{
  a->append('a');
  CPPUNIT_ASSERT(strcmp(a->c_str(), "a") == 0);
  a->append("bcd");
  CPPUNIT_ASSERT(*a == "abcd");
  a->append("test", 0);
  CPPUNIT_ASSERT(*a == "abcd");
  f->append(&cstring[11], strlen(cstring) - 11);
  CPPUNIT_ASSERT(*f == *d);
  c->append(*b);
  CPPUNIT_ASSERT(*c == "blahblah");
  c->append(' ');
  c->append(*c);
  CPPUNIT_ASSERT(*c == "blahblah blahblah ");
  c->append("  ");
  c->append(*b);
  CPPUNIT_ASSERT(*c == "blahblah blahblah   blah");
}

void StringTest :: insertTest(void)
{
  a->insert(1, 'a');
  CPPUNIT_ASSERT_EQUAL(false, a->hasIndex(0));
  a->insert(0, 'b');
  CPPUNIT_ASSERT(*a == "b");
  CPPUNIT_ASSERT_EQUAL(false, a->hasIndex(1));
  b->insert(2, "BLAH");
  CPPUNIT_ASSERT(*b == "blBLAHah");
  CPPUNIT_ASSERT(*c == "blah");
  b->insert(3, "TEST", 0);
  CPPUNIT_ASSERT(*b == "blBLAHah");
  CPPUNIT_ASSERT(*c == "blah");
  b->insert(3, "TEST", 1);
  CPPUNIT_ASSERT(*b == "blBTLAHah");
  CPPUNIT_ASSERT(*c == "blah");
}

void StringTest :: replaceTest(void)
{
  a->replace(1, "wtf");
  CPPUNIT_ASSERT_EQUAL(false, a->hasIndex(0));
  b->replace(0, "haha");
  CPPUNIT_ASSERT(*b == "haha");
  CPPUNIT_ASSERT(*c == "blah");
  g->replace(1, "ml");
  CPPUNIT_ASSERT(*g == "xml");
}

void StringTest :: concatTest(void)
{
 *a = *b + " " + *b;
 CPPUNIT_ASSERT(*a == "blah blah");
 *a = *b + *a;
 CPPUNIT_ASSERT(*a == "blahblah blah");
 delete a;
 a = new String();
 *a = *b + *a;
 CPPUNIT_ASSERT(*a == "blah");
}

void StringTest :: incDecTest(void)
{
  *a = "test";
  CPPUNIT_ASSERT(*a == "test");
  String old = (*a)++;
  CPPUNIT_ASSERT(old == "test");
  CPPUNIT_ASSERT(*a == "est");
  CPPUNIT_ASSERT_EQUAL((size_t) 3, a->length());
  ++(*a);
  CPPUNIT_ASSERT(*a == "st");
  CPPUNIT_ASSERT_EQUAL((size_t) 2, a->length());
  old = (*a)--;
  CPPUNIT_ASSERT(old == "st");
  CPPUNIT_ASSERT(*a == "s");
  CPPUNIT_ASSERT_EQUAL((size_t) 1, a->length());
  --(*a);
  CPPUNIT_ASSERT(*a == "");
  CPPUNIT_ASSERT_EQUAL((size_t) 0, a->length());
}

void StringTest :: incDecEqualTest(void)
{
  *a += "longtest";
  CPPUNIT_ASSERT(*a == "longtest");
  *a -= 3;
  CPPUNIT_ASSERT(*a == "longt");
  *a += 4;
  CPPUNIT_ASSERT(*a == "t");
  CPPUNIT_ASSERT_EQUAL((size_t) 1, a->length());
  *a += *b;
  CPPUNIT_ASSERT(*a == "tblah");
  CPPUNIT_ASSERT_EQUAL((size_t) 5, a->length());

  *d += *d + *d;
  char check[100];
  strcpy(check, cstring); // *d = cstring
  strcat(check, cstring); 
  strcat(check, cstring);
  CPPUNIT_ASSERT(*d == check);

  *b += *a;
  *c = *c + *a;

  CPPUNIT_ASSERT(*b == "blahtblah");
  CPPUNIT_ASSERT(*c == "blahtblah");
  CPPUNIT_ASSERT(*b == *c);
}

void StringTest :: printfTest(void)
{
  int n = 506;
  const char* s = "something";

  a->printf("%s %d", s, n);
  CPPUNIT_ASSERT(*a == "something 506");
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

void StringTest :: base64Test(void)
{
  b->base64Encode();
  CPPUNIT_ASSERT(*b != *c);
  CPPUNIT_ASSERT(*b != "blah");
  CPPUNIT_ASSERT(*b == "MalVO...");
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
