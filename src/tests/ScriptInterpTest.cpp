/* ScriptInterpTest.c
 *
 */
#include "ScriptInterpTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (ScriptInterpTest);

void ScriptInterpTest :: setUp (void)
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
}

void ScriptInterpTest :: tearDown (void)
{
    // finally delete objects
//    delete a; delete b; delete c; delete d;
//    delete e; delete f; delete g; delete h;
}

void ScriptInterpTest :: evalTest (void)
{
  ScriptInterpTCL tcl_script;
  tcl_script.init();

  CPPUNIT_ASSERT_STRING_EQUAL("8", tcl_script.eval("expr {4 + 4}"));
  CPPUNIT_ASSERT_STRING_EQUAL(tcl_script.eval("set errorInfo"), tcl_script.eval("unknown"));
}
