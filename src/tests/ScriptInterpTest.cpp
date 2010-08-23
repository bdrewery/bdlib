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

  CPPUNIT_ASSERT_STRING_EQUAL("8", tcl_script.eval("expr {4 + 4}"));
  CPPUNIT_ASSERT_STRING_EQUAL(tcl_script.eval("set errorInfo"), tcl_script.eval("unknown"));
}

/*
void ScriptInterpTest :: operatorEqualsTest (void)
{
  ScriptInterpTCL tcl_script;
  tcl_script.eval("set x 5");

  CPPUNIT_ASSERT_STRING_EQUAL("5", tcl_script.eval("set x"));
  ScriptInterpTCL tcl_script2;
  tcl_script2.eval("set x 1");
  CPPUNIT_ASSERT_STRING_EQUAL("1", tcl_script2.eval("set x"));


  tcl_script2 = tcl_script;

  CPPUNIT_ASSERT_STRING_EQUAL("5", tcl_script.eval("set x"));
  CPPUNIT_ASSERT_STRING_EQUAL("5", tcl_script2.eval("set x"));
}
*/
void ScriptInterpTest :: linkVarTest (void)
{
  ScriptInterpTCL tcl_script;

  String x("54321");
  tcl_script.linkVar("x", x);


  // Test Getting var from C++
  CPPUNIT_ASSERT_STRING_EQUAL(x, tcl_script.eval("set x"));
  x = "test 1 2 3 4 5 ";
  CPPUNIT_ASSERT_STRING_EQUAL(x, tcl_script.eval("set x"));

  // Test Setting var from TCL
  String newval("5 4 2 3 1  test");

  tcl_script.eval(String("set x \"") + newval + "\"");
  CPPUNIT_ASSERT_STRING_EQUAL(newval, tcl_script.eval("set x"));

  // Test getting a substring
  x = newval(0, 5);
  CPPUNIT_ASSERT_STRING_EQUAL(x, tcl_script.eval("set x"));

}
