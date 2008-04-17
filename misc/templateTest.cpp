/* @FILE@Test.c
 *
 * $Id: $
 */
#include "@FILE@Test.h"

CPPUNIT_TEST_SUITE_REGISTRATION (@FILE@Test);

void @FILE@Test :: setUp (void)
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
  a = new @FILE@<String>();
  b = new @FILE@<String>();
  c = new @FILE@<String>();
  d = new @FILE@<String>();
  e = new @FILE@<String*>();
  f = new @FILE@<String*>();
  g = new @FILE@<String*>();
  h = new @FILE@<String*>();
}

void @FILE@Test :: tearDown (void)
{
    // finally delete objects
    delete a; delete b; delete c; delete d;
    delete e; delete f; delete g; delete h;
}

