/* ScriptInterpTCL.cpp
 *
 */
#include "ScriptInterpTCL.h"
#include <stdarg.h>
#include <algorithm> // min() / max()
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

BDLIB_NS_BEGIN

int ScriptInterpTCL::init() {
  // create interp
  interp = Tcl_CreateInterp();
  //Tcl_FindExecutable(binname);

  if (Tcl_Init(interp) != TCL_OK) {
    fprintf(stderr, "Tcl_Init error: %s\n", Tcl_GetStringResult(interp));
    return 1;
  }
  return 0;
}

int ScriptInterpTCL::destroy() {
  Tcl_DeleteInterp(interp);
  return 0;
}

String ScriptInterpTCL::eval(const String& script) {
  if (Tcl_Eval(interp, script.c_str()) == TCL_OK) {
    return Tcl_GetStringResult(interp);
  } else
    return eval("set errorInfo");
  return String();
}

BDLIB_NS_END
