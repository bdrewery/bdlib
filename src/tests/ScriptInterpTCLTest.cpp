/* ScriptInterpTCLTest.c
 *
 */
#include "ScriptInterpTCLTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (ScriptInterpTCLTest);

void ScriptInterpTCLTest :: setUp (void)
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

void ScriptInterpTCLTest :: tearDown (void)
{
    // finally delete objects
//    delete a; delete b; delete c; delete d;
//    delete e; delete f; delete g; delete h;
}

void ScriptInterpTCLTest :: evalTest (void)
{
  ScriptInterpTCL tcl_script;

  CPPUNIT_ASSERT_STRING_EQUAL("8", tcl_script.eval("expr {4 + 4}"));
  CPPUNIT_ASSERT_STRING_EQUAL(tcl_script.eval("set errorInfo"), tcl_script.eval("unknown"));
}

/*
void ScriptInterpTCLTest :: operatorEqualsTest (void)
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
void ScriptInterpTCLTest :: linkVarTest (void)
{
  ScriptInterpTCL tcl_script;

  /* Strings */

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
  CPPUNIT_ASSERT_STRING_EQUAL(newval, x);

  // Test getting a substring
  x = newval(0, 5);
  CPPUNIT_ASSERT_STRING_EQUAL(x, tcl_script.eval("set x"));

  // Test readonly var
  const String ro("I am read-only");
  tcl_script.linkVar("ro", ro);
  CPPUNIT_ASSERT_STRING_EQUAL(ro, tcl_script.eval("set ro"));
  tcl_script.eval("set ro \"test\"");
  CPPUNIT_ASSERT_STRING_EQUAL("I am read-only", tcl_script.eval("set ro"));
  CPPUNIT_ASSERT_STRING_EQUAL("I am read-only", ro);


  /* Ints */

  int y = 42;
  tcl_script.linkVar("y", y);
  CPPUNIT_ASSERT_EQUAL(y, atoi(tcl_script.eval("set y").c_str()));
  CPPUNIT_ASSERT_EQUAL(46, atoi(tcl_script.eval("set y [expr {$y + 4}]").c_str()));
  CPPUNIT_ASSERT_EQUAL(46, y);

  // Set from C
  y = 503;
  CPPUNIT_ASSERT_EQUAL(y, atoi(tcl_script.eval("set y").c_str()));

  // Test const
  const int cy = 52;
  tcl_script.linkVar("cy", cy);
  CPPUNIT_ASSERT_EQUAL(cy, atoi(tcl_script.eval("set cy").c_str()));
  tcl_script.eval("set cy 12");
  CPPUNIT_ASSERT_EQUAL(52, atoi(tcl_script.eval("set cy").c_str()));
  CPPUNIT_ASSERT_EQUAL(52, cy);

  /* Longs */

  long ly = 42;
  tcl_script.linkVar("ly", ly);
  CPPUNIT_ASSERT_EQUAL(ly, atol(tcl_script.eval("set ly").c_str()));
  CPPUNIT_ASSERT_EQUAL((long)46, atol(tcl_script.eval("set ly [expr {$ly + 4}]").c_str()));
  CPPUNIT_ASSERT_EQUAL((long)46, ly);

  // Set from C
  ly = 503;
  CPPUNIT_ASSERT_EQUAL(ly, atol(tcl_script.eval("set ly").c_str()));

  // Test const
  const long lcy = 52;
  tcl_script.linkVar("lcy", lcy);
  CPPUNIT_ASSERT_EQUAL(lcy, atol(tcl_script.eval("set lcy").c_str()));
  tcl_script.eval("set lcy 12");
  CPPUNIT_ASSERT_EQUAL((long)52, atol(tcl_script.eval("set lcy").c_str()));
  CPPUNIT_ASSERT_EQUAL((long)52, lcy);

  /* Doubles */

  double dy = 42.8;
  tcl_script.linkVar("dy", dy);
  CPPUNIT_ASSERT_STRING_EQUAL("42.8", tcl_script.eval("set dy"));
  CPPUNIT_ASSERT_STRING_EQUAL("46.8", tcl_script.eval("set dy [expr {$dy + 4}]"));
  CPPUNIT_ASSERT_EQUAL((double)46.8, dy);

  // Set from C
  dy = 87.5;
  CPPUNIT_ASSERT_STRING_EQUAL("87.5", tcl_script.eval("set dy"));

  // Test const
  const double dcy = 52.2;
  tcl_script.linkVar("dcy", dcy);
  CPPUNIT_ASSERT_STRING_EQUAL("52.2", tcl_script.eval("set dcy"));
  tcl_script.eval("set dcy 12");
  CPPUNIT_ASSERT_STRING_EQUAL("52.2", tcl_script.eval("set dcy"));
  CPPUNIT_ASSERT_EQUAL((double)52.2, dcy);

  bool by = 1;
  tcl_script.linkVar("by", by);
  CPPUNIT_ASSERT_EQUAL(by, (bool)atol(tcl_script.eval("set by").c_str()));
  CPPUNIT_ASSERT_EQUAL((bool)1, by);

  CPPUNIT_ASSERT_EQUAL((bool)1, (bool)atol(tcl_script.eval("set by 2").c_str()));


  // Set from C
  by = 0;
  CPPUNIT_ASSERT_EQUAL(by, (bool)atol(tcl_script.eval("set by").c_str()));

  // Test const
  const bool bcy = 1;
  tcl_script.linkVar("bcy", bcy);
  CPPUNIT_ASSERT_EQUAL(bcy, (bool)atol(tcl_script.eval("set bcy").c_str()));
  tcl_script.eval("set bcy 0");
  CPPUNIT_ASSERT_EQUAL((bool)1, (bool)atol(tcl_script.eval("set bcy").c_str()));
  CPPUNIT_ASSERT_EQUAL((bool)1, bcy);
}

String my_x(ScriptInterp& interp, const ScriptArgs& args, ScriptInterp::script_clientdata_t clientData) {String my_cd = (clientData ? *(String*) clientData : String()); return "Test command proc" + my_cd;}
int my_xi(ScriptInterp& interp, const ScriptArgs& args, ScriptInterp::script_clientdata_t clientData) {String my_cd = (clientData ? *(String*) clientData : String("0")); return 53 + atoi(*my_cd);}
String param_test(ScriptInterp& interp, const ScriptArgs& args, ScriptInterp::script_clientdata_t clientData) {
  String my_cd = (clientData ? *(String*) clientData : String());
  String arg1 = args.getArgString(1);
  if (args.length() > 2) {
    int arg2 = args.getArgInt(2);
    return String::printf("I got %zu args, arg1: %s arg2: %d", args.length() - 1, *arg1, arg2);
  } else
    return String::printf("I got %zu args, arg1: %s", args.length() - 1, *arg1);
}

void ScriptInterpTCLTest :: createCommandTest (void)
{
  ScriptInterpTCL tcl_script;

  tcl_script.createCommand("x", my_x);
  CPPUNIT_ASSERT_STRING_EQUAL(my_x(tcl_script, ScriptArgsTCL(), NULL), tcl_script.eval("x"));

  String my_cd("my cd");
  tcl_script.createCommand("y", my_x, &my_cd);
  CPPUNIT_ASSERT_STRING_EQUAL(my_x(tcl_script, ScriptArgsTCL(), &my_cd), tcl_script.eval("y"));

  // Test int return

  tcl_script.createCommand("xi", my_xi);
  CPPUNIT_ASSERT_EQUAL(my_xi(tcl_script, ScriptArgsTCL(), NULL), atoi(*tcl_script.eval("xi")));
  CPPUNIT_ASSERT_EQUAL(53, atoi(*tcl_script.eval("xi")));

  my_cd = "7";
  tcl_script.createCommand("yi", my_xi, &my_cd);
  CPPUNIT_ASSERT_EQUAL(my_xi(tcl_script, ScriptArgsTCL(), &my_cd), atoi(*tcl_script.eval("yi")));
  CPPUNIT_ASSERT_EQUAL(60, atoi(*tcl_script.eval("yi")));

  tcl_script.createCommand("param_test", param_test);
  CPPUNIT_ASSERT_STRING_EQUAL("I got 1 args, arg1: TEST", tcl_script.eval("param_test \"TEST\""));
  CPPUNIT_ASSERT_STRING_EQUAL("I got 2 args, arg1: TEST arg2: 1", tcl_script.eval("param_test \"TEST\" 1"));
  CPPUNIT_ASSERT_STRING_EQUAL("I got 1 args, arg1: TeSt", tcl_script.eval("param_test \"TeSt\""));
  CPPUNIT_ASSERT_STRING_EQUAL("I got 1 args, arg1: 5", tcl_script.eval("param_test 5"));

}

void ScriptInterpTCLTest :: deleteCommandTest (void)
{
  ScriptInterpTCL tcl_script;
  String bad_cmd = tcl_script.eval("x");

  tcl_script.createCommand("x", my_x);
  CPPUNIT_ASSERT_STRING_EQUAL(my_x(tcl_script, ScriptArgsTCL(), NULL), tcl_script.eval("x"));
  tcl_script.deleteCommand("x");
  CPPUNIT_ASSERT_STRING_EQUAL(bad_cmd, tcl_script.eval("x"));
}
