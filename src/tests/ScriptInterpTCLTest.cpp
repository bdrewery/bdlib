/* ScriptInterpTCLTest.c
 *
 */
#include <unistd.h>
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
  tcl_script.eval("unknown");
  CPPUNIT_ASSERT_STRING_EQUAL(tcl_script.eval("set errorInfo"), tcl_script.eval("unknown"));
}

void ScriptInterpTCLTest :: loadScriptTest (void)
{
  ScriptInterpTCL tcl_script;
  String bad_cmd = tcl_script.eval("x");
  String resultStr;

  CPPUNIT_ASSERT_STRING_EQUAL(bad_cmd, tcl_script.eval("x"));
  CPPUNIT_ASSERT_EQUAL(ScriptInterp::SCRIPT_LOAD_WRONG_INTERP, tcl_script.loadScript("/dev/null", resultStr));

  // Now create/load script with proc x and verify cmd exists

  Stream test_tcl;
  String fileName("test.tcl");
  String expected_result = "x proc";
  test_tcl << "proc x {} { return \"" + expected_result + "\"}";
  CPPUNIT_ASSERT_EQUAL(ScriptInterp::SCRIPT_LOAD_ERROR, tcl_script.loadScript(fileName, resultStr));
  unlink(*fileName);
  test_tcl.writeFile(fileName);
  CPPUNIT_ASSERT_EQUAL(ScriptInterp::SCRIPT_LOAD_OK, tcl_script.loadScript(fileName, resultStr));
  CPPUNIT_ASSERT_STRING_EQUAL(expected_result, tcl_script.eval("x"));
  unlink(*fileName);
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

static String changed_nick_oldval;
static String changed_nick_newval;
void changed_nick(const String* oldval, const String *newval) {
  changed_nick_oldval = *oldval;
  changed_nick_newval = *newval;
}

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

  // Test templates
  ScriptInterp::linkVar(tcl_script, "t_x", x);
  CPPUNIT_ASSERT_STRING_EQUAL(x, tcl_script.eval("set t_x"));

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

  // Test templates
  ScriptInterp::linkVar(tcl_script, "t_y", y);
  CPPUNIT_ASSERT_EQUAL(y, atoi(tcl_script.eval("set t_y").c_str()));

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

  /* XXX: Test invalid type conversion */

  /* Test hooked vars */
  String nick("oldval");
  tcl_script.linkVar("nick", nick, reinterpret_cast<ScriptInterp::link_var_hook>(changed_nick));
  CPPUNIT_ASSERT_STRING_EQUAL("oldval", nick);
  CPPUNIT_ASSERT_EQUAL(true, changed_nick_oldval.isEmpty());
  CPPUNIT_ASSERT_EQUAL(true, changed_nick_newval.isEmpty());
  tcl_script.eval("set nick newval");
  CPPUNIT_ASSERT_STRING_EQUAL("oldval", changed_nick_oldval);
  CPPUNIT_ASSERT_STRING_EQUAL("newval", changed_nick_newval);
  CPPUNIT_ASSERT_STRING_EQUAL("newval", nick);
}

void ScriptInterpTCLTest::unlinkVarTest(void) {
  ScriptInterpTCL tcl_script;
  String varNotFound(tcl_script.eval("set x"));
  int x = 5;

  tcl_script.linkVar("x", x);
  // Verify it exists
  CPPUNIT_ASSERT_EQUAL(5, atoi(*tcl_script.eval("set x")));

  tcl_script.unlinkVar("x");
  // Verify it does't exist
  CPPUNIT_ASSERT_STRING_EQUAL(varNotFound, tcl_script.eval("set x"));
}

String my_x(String my_cd) {
  return "Test command proc" + my_cd;
}

int my_xi(String my_cd) {
  return 53 + atoi(*my_cd);
}

bool my_xz(const String my_cd, int my_i, bool my_bool, String my_string) {
  return true;
}

void voidFunc() {
}

String param_test(String arg1, int arg2) {
  String return_string;
  if (arg2) {
    return_string = String::printf("I got %zu args, arg1: %s arg2: %d", size_t(2), *arg1, arg2);
  } else {
    return_string = String::printf("I got %zu args, arg1: %s", size_t(1), *arg1);
  }
  return return_string;
}

void args(int foo1, int foo2) {
  if (foo1 == -1)
    throw bd::String("Invalid param: -1");
  else if (foo1 == -2)
    throw std::exception();
}

Array<String> listReturn(String arg1, String arg2, String arg3) {
  Array<String> ret = {arg1, arg2, arg3};
  return ret;
}

Array<Array<String>> nestedListReturn(String arg1, String arg2, String arg3) {
  Array<Array<String>> ret;
  ret << Array<String>{arg1, "10"};
  ret << Array<String>{arg2, "20"};
  ret << Array<String>{arg3, "30"};
  return ret;
}

String listParam(Array<String> list, String param) {
  return param + " " + list.join("-");
}

void ScriptInterpTCLTest :: createCommandTest (void)
{
  ScriptInterpTCL tcl_script;
  String result;

  tcl_script.createCommand("x", my_x);
  result = String::printf("Wrong # args. Expected 1, got 0.\n    while executing\n\"x\"");
  CPPUNIT_ASSERT_STRING_EQUAL(result, tcl_script.eval("x"));

  tcl_script.createCommand("x", my_x);
  result = my_x("Test");
  CPPUNIT_ASSERT_STRING_EQUAL(result, tcl_script.eval("x Test"));

  // Test int return

  tcl_script.createCommand("xi", my_xi);
  CPPUNIT_ASSERT_EQUAL(my_xi("0"), atoi(*tcl_script.eval("xi 0")));
  CPPUNIT_ASSERT_EQUAL(my_xi("1"), atoi(*tcl_script.eval("xi 1")));
  CPPUNIT_ASSERT_EQUAL(53, atoi(*tcl_script.eval("xi \"0\"")));

  // Test dispatch through ScriptInterp
  ScriptInterp::createCommand(tcl_script, "param_test", param_test, nullptr, 1);
  // Try too little
  result = String::printf("Wrong # args. Expected 1, got 0.\n    while executing\n\"param_test\"");
  CPPUNIT_ASSERT_STRING_EQUAL(result, tcl_script.eval("param_test"));
  // Try too many
  result = String::printf("Wrong # args. Expected 1, got 3.\n    while executing\n\"param_test \"TEST\" 1 2\"");
  CPPUNIT_ASSERT_STRING_EQUAL(result, tcl_script.eval("param_test \"TEST\" 1 2"));
  CPPUNIT_ASSERT_STRING_EQUAL("I got 2 args, arg1: TEST arg2: 1", tcl_script.eval("param_test \"TEST\" 1"));
  CPPUNIT_ASSERT_STRING_EQUAL("I got 2 args, arg1: 5 arg2: 10", tcl_script.eval("param_test 5 10"));

  // Test [list] return
  tcl_script.createCommand("listReturn", listReturn);
  CPPUNIT_ASSERT_STRING_EQUAL("1 2 3", tcl_script.eval("listReturn 1 2 3"));

  // Test [list {} {} {}] return
  tcl_script.createCommand("nestedListReturn", nestedListReturn);
  CPPUNIT_ASSERT_STRING_EQUAL("{1 10} {2 20} {3 30}", tcl_script.eval("nestedListReturn 1 2 3"));

  // Test [list] param
  tcl_script.createCommand("listParam", listParam);
  CPPUNIT_ASSERT_STRING_EQUAL("list 1-2-3", tcl_script.eval("listParam {1 2 3} list"));

  // Test default params
  CPPUNIT_ASSERT_STRING_EQUAL("I got 1 args, arg1: TEST", tcl_script.eval("param_test \"TEST\""));
  // Check extern functional
  tcl_script.createCommand("atoi", atoi);
  CPPUNIT_ASSERT_EQUAL(atoi("5"), atoi(*tcl_script.eval("atoi 5")));
  CPPUNIT_ASSERT_EQUAL(5, atoi(*tcl_script.eval("atoi 5")));

  // Mostly a compile test on the templates
  tcl_script.createCommand("xz", my_xz);
  tcl_script.createCommand("voidFunc", voidFunc);

  tcl_script.createCommand("args", args, "foo1 ?foo2?", 1);
  CPPUNIT_ASSERT_STRING_EQUAL("Wrong # args. Expected 1, got 0. Should "
      "be \"args foo1 ?foo2?\"\n    while executing\n\"args\"", tcl_script.eval("args"));

  CPPUNIT_ASSERT_STRING_EQUAL("Invalid param: -1\n    "
      "while executing\n\"args -1\"", tcl_script.eval("args -1"));

  CPPUNIT_ASSERT_STRING_EQUAL("Unhandled exception.\n    "
      "while executing\n\"args -2\"", tcl_script.eval("args -2"));
}

HashTable<String, ScriptCallbacker* > Events;

void script_bind(String eventName, ScriptCallbacker* scb) {
  // s:event c:Proc
  Events[eventName] = scb;
}

void ScriptInterpTCLTest :: createCommandEventTest (void)
{
  ScriptInterpTCL tcl_script;
  ScriptCallbacker* scb;
  String result, input;
  Array<String> params;

  tcl_script.createCommand("param_test", param_test);

  /* Try a TCL Callback from C++ (event binding from C)
   * Execute script_bind from TCL, which will record the bound event callback in Events
   * Then call the Event and verify that it returns our expected result
   */
  // Create script_bind which will add the passed proc to our event handling procedures (Just an Array for now)
  tcl_script.createCommand("bind", script_bind);
  // Bind an event
  tcl_script.eval("proc echo {args} { return $args }");
  tcl_script.eval("bind test echo");

  // Call the event from C++ and then verify it echos back the input
  scb = Events["test"];
  input = "my params";
  params << input;
  // FIXME: This call line needs help to accept arbitrary params
  result = scb->call(params);
  CPPUNIT_ASSERT_STRING_EQUAL(bd::String::printf("{%s}", input.c_str()), result);
  params.clear();

  tcl_script.eval("bind \"complex test\" param_test");
  scb = Events["complex test"];

  // Now trigger the callback and verify that it calls the passed event
  params << "test";
  params << "42";
  result = scb->call(params);
  CPPUNIT_ASSERT_STRING_EQUAL("I got 2 args, arg1: test arg2: 42", result);
  params.clear();

  params << "some argument";
  params << "45";
  result = scb->call(params);
  CPPUNIT_ASSERT_STRING_EQUAL("I got 2 args, arg1: some argument arg2: 45", result);
}

void ScriptInterpTCLTest :: deleteCommandTest (void)
{
  ScriptInterpTCL tcl_script;
  String bad_cmd = tcl_script.eval("x");

  tcl_script.createCommand("x", my_x);
  CPPUNIT_ASSERT_STRING_EQUAL("Test command proctest", tcl_script.eval("x \"test\""));
  tcl_script.deleteCommand("x");
  CPPUNIT_ASSERT_STRING_EQUAL(bad_cmd, tcl_script.eval("x"));
}
