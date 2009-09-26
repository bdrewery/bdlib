/* StreamTest.c
 *
 */
#include "StreamTest.h"
#include <cstring>
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION (StreamTest);

void StreamTest :: setUp (void)
{
    strcpy(cstring, "Some static cstring to play with");
    // set up test environment (initializing objects)
    a = new Stream();
    b = new Stream("blah");
    c = new Stream(*b);
    d = new Stream(cstring);
    e = new Stream(*d);
    f = new Stream(cstring, 11);
    g = new Stream('x');
    h = new Stream(35);
}

void StreamTest :: tearDown (void)
{
    // finally delete objects
    delete a; delete b; delete c; delete d;
    delete e; delete f; delete g; delete h;
}

void StreamTest :: reserveTest (void)
{
  a->Reserve(2000);
  CPPUNIT_ASSERT((a->capacity() % 2048) == 0);

  b->Reserve(10);
  CPPUNIT_ASSERT(b->capacity() >= 1024);
  CPPUNIT_ASSERT(h->capacity() >= 1024);
  CPPUNIT_ASSERT((b->capacity() % 1024) == 0);

  char buf[2048];
  memset(buf, 'c', 2048);
  g->puts(buf);
  CPPUNIT_ASSERT((g->capacity() % 1024) == 0);

  /* Reference counted */
  b->puts(buf);
  CPPUNIT_ASSERT((b->capacity() % 1024) == 0);
}

void StreamTest :: tellTest (void)
{
  CPPUNIT_ASSERT_EQUAL((size_t) 0, a->tell());
  CPPUNIT_ASSERT_EQUAL((size_t) 0, b->tell());
  CPPUNIT_ASSERT_EQUAL((size_t) 0, h->tell());
}

void StreamTest :: seekTest (void)
{
  a->seek(0, SEEK_END);
  CPPUNIT_ASSERT_EQUAL((size_t) 0, a->tell());

  b->seek(0, SEEK_END);
  CPPUNIT_ASSERT_EQUAL(b->capacity(), b->tell());

  b->seek(0, SEEK_SET);
  CPPUNIT_ASSERT_EQUAL((size_t) 0, b->tell());

  b->seek(1, SEEK_SET);
  CPPUNIT_ASSERT_EQUAL((size_t) 1, b->tell());

  b->seek(1, SEEK_END);
  CPPUNIT_ASSERT_EQUAL(b->capacity() - 1, b->tell());

}

void StreamTest :: putsTest (void)
{
  a->puts("This is a test of my buffer\n");
  a->puts("This is line 2, isn't it amazing?\n");
  CPPUNIT_ASSERT(*a == "This is a test of my buffer\nThis is line 2, isn't it amazing?\n");
  CPPUNIT_ASSERT_EQUAL((size_t) 62, a->length());
  *b = *a;
  a->seek(10, SEEK_SET);
  a->puts("look");
  CPPUNIT_ASSERT(*a == "This is a look of my buffer\nThis is line 2, isn't it amazing?\n");
  CPPUNIT_ASSERT_EQUAL((size_t) 62, a->length());
  CPPUNIT_ASSERT_EQUAL(b->capacity(), a->capacity());
  CPPUNIT_ASSERT_EQUAL(b->length(), a->length());

  a->clear();
  b->clear();

  *a << "This is a test of my buffer\n";
  *a << "This is line 2, isn't it amazing?\n";
  CPPUNIT_ASSERT(*a == "This is a test of my buffer\nThis is line 2, isn't it amazing?\n");
  CPPUNIT_ASSERT_EQUAL((size_t) 62, a->length());
  *b = *a;
  a->seek(10, SEEK_SET);
  *a << "look";
  CPPUNIT_ASSERT(*a == "This is a look of my buffer\nThis is line 2, isn't it amazing?\n");
  CPPUNIT_ASSERT_EQUAL((size_t) 62, a->length());
  CPPUNIT_ASSERT_EQUAL(b->capacity(), a->capacity());
  CPPUNIT_ASSERT_EQUAL(b->length(), a->length());
}

void StreamTest :: truncateTest (void)
{
  a->puts("This is a test of my buffer\n");
  a->puts("This is line 2, isn't it amazing?\n");
  a->clear();
  CPPUNIT_ASSERT_EQUAL((size_t) 0, a->length());
  CPPUNIT_ASSERT(*a == "");

  a->puts("This is a test of my buffer\n");
  a->puts("This is line 2, isn't it amazing?\n");
  a->seek(10, SEEK_SET);
  a->truncate();
  CPPUNIT_ASSERT_EQUAL((size_t) 10, a->length());
  CPPUNIT_ASSERT(*a == "This is a ");
}

void StreamTest :: getsTest (void)
{
  char buf[51];

  a->puts("This is line 1\n");
  a->puts("This is line 2\n");
  a->puts("This is line 3\n");
  a->puts("This is line 4\n");
  a->puts("This is line 5\n");
  a->puts("This is line 6\n");
  a->puts("This is line 7\n");
  a->puts("This is line 8\n");
  a->puts("This is line 9\n");
  a->puts("This is line10\n");
  a->puts("This is line11\n");
  a->puts("This is line12\n");
  a->puts("This is line13\n");
  a->seek(0, SEEK_SET);
  int x = 0;
  size_t pos = 0, size = 0;
  while (a->tell() < a->length()) {
    ++x;
    size = a->gets(buf, sizeof(buf));
    pos += size;
    CPPUNIT_ASSERT_EQUAL(pos, a->tell());
    CPPUNIT_ASSERT_EQUAL((size_t) 15, strlen(buf));
    CPPUNIT_ASSERT_EQUAL((size_t) 15, size);
  }
  CPPUNIT_ASSERT_EQUAL(13, x);
}

void StreamTest :: printfTest (void)
{
  char buf[51];

  a->printf("This is line 1\n");
  a->printf("This is line 2\n");
  a->printf("This is line 3\n");
  a->printf("This is line 4\n");
  a->printf("This is line 5\n");
  a->printf("This is line 6\n");
  a->printf("This is line 7\n");
  a->printf("This is line 8\n");
  a->printf("This is line 9\n");
  a->printf("This is line10\n");
  a->printf("This is line11\n");
  a->printf("This is line12\n");
  a->printf("This is line13\n");
  a->seek(0, SEEK_SET);
  int x = 0;
  size_t pos = 0, size = 0;;
  while (a->tell() < a->length()) {
    ++x;
    size = a->gets(buf, sizeof(buf));
    pos += size;
    CPPUNIT_ASSERT_EQUAL(pos, a->tell());
    CPPUNIT_ASSERT_EQUAL((size_t) 15, strlen(buf));
    CPPUNIT_ASSERT_EQUAL((size_t) 15, size);
  }
  CPPUNIT_ASSERT_EQUAL(13, x);
}

void StreamTest :: loadFileTest (void)
{
  const char *file = "/etc/passwd";

  FILE *f = NULL;
  f = fopen(file, "rb");
  if (f == NULL)
    return;

  fseek(f, 0, SEEK_END);
  size_t size = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *buffer = (char*) malloc(size + 1);
  fread(buffer, 1, size, f);
  buffer[size] = 0;

  a->loadFile(file);

  CPPUNIT_ASSERT_EQUAL(size, a->length());
  CPPUNIT_ASSERT(*a == buffer);
  free(buffer);

  char buf[1024] = "";
  char sbuf[1024] = "";
  size_t len = 0;

  fseek(f, 0, SEEK_SET);
  while (fgets(buf, sizeof(buf), f) != NULL) {
    len = a->gets(sbuf, sizeof(sbuf));
    CPPUNIT_ASSERT_EQUAL(0, strcmp(buf, sbuf));
//std::cout << sbuf << std::endl;
//std::cout << buf << std::endl;
  }
  fclose(f);


//  char buf[1024];
//  while (a->tell() < a->length()) {
//    a->gets(buf, sizeof(buf));
//  }
}
