/* coreTest.c
 *
 * $Id$
 */
#include "coreTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (coreTest);

void coreTest :: setUp (void)
{
    // set up test environment (initializing objects)
    char *argv_1[1] = {"/bryan/testBin"};
    char *argv_2[2] = {"./testBin", "-x"};
    char *argv_3[3] = {"./testBin", "-x", "box"};
    a = new Core(sizeof(argv_1), argv_1);
    b = new Core(sizeof(argv_2), argv_2);
    c = new Core(sizeof(argv_3), argv_3);
}

void coreTest :: tearDown (void)
{
    // finally delete objects
    delete a; 
    delete b; 
    delete c; 
}

#include <unistd.h>
void coreTest :: getBinnameTest (void)
{
  char buf[1024] = "";
  getcwd(buf, sizeof(buf));
  strcat(buf, "/testBin");
#ifdef CYGWIN_HACKS
  strcat(buf, ".exe");
  CPPUNIT_ASSERT(a->getBinname() == "/bryan/testBin.exe");
#else
  CPPUNIT_ASSERT(a->getBinname() == "/bryan/testBin");
#endif /* CYGWIN_HACKS */
  CPPUNIT_ASSERT(b->getBinname() == buf);
  CPPUNIT_ASSERT(c->getBinname() == buf);
}
