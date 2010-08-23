/* ScriptInterpTCL.cpp
 *
 */
#include "ScriptInterpTCL.h"
#include <stdarg.h>
#include <algorithm> // min() / max()
#include <limits.h>
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
  if (interp && !Tcl_InterpDeleted(interp)) {
    Tcl_DeleteInterp(interp);
    interp = NULL;
  }
  return 0;
}

String ScriptInterpTCL::eval(const String& script) {
  if (Tcl_Eval(interp, script.c_str()) == TCL_OK) {
    return Tcl_GetStringResult(interp);
  } else
    return eval("set errorInfo");
  return String();
}

const char* ScriptInterpTCL::TraceSetRO (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  return "variable is read-only";
}

const char* ScriptInterpTCL::TraceGetString (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  String* str = (String*) clientData;
  Tcl_Obj *value = (str->length() < INT_MAX) ? Tcl_NewStringObj(str->data(), str->length()) : NULL;

  if (value) {
    Tcl_SetVar2(interp,name1,name2,Tcl_GetStringFromObj(value,NULL), flags);
    Tcl_DecrRefCount(value);
  }
  return NULL;
}

const char* ScriptInterpTCL::TraceSetString (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  Tcl_Obj *value = NULL;
  Tcl_Obj *name1o = NULL;

  name1o = Tcl_NewStringObj(name1,-1);
  value = Tcl_ObjGetVar2(interp, name1o, 0, flags);
  Tcl_DecrRefCount(name1o);
  if (!value) goto fail;
  {
    int len = 0;
    char *cstr = Tcl_GetStringFromObj(value, &len);
    if (!cstr) {
//      SWIG_exception_fail(SWIG_ArgError(res), "in variable '""server_list""' of type '""char [256]""'");
      goto fail;
    }
    String* str = (String*) clientData;
    (*str) = String(cstr, len);
  }
  return NULL;
fail:
  return name1;
}

void ScriptInterpTCL::linkVar(const String& name, String& var) {
  Tcl_SetVar(interp, *name, "", TCL_GLOBAL_ONLY);
  Tcl_TraceVar(interp, *name, TCL_TRACE_READS | TCL_GLOBAL_ONLY, (Tcl_VarTraceProc *) this->TraceGetString, (ClientData) &var);
  Tcl_TraceVar(interp, *name, TCL_TRACE_WRITES | TCL_GLOBAL_ONLY, (Tcl_VarTraceProc *) this->TraceSetString, (ClientData) &var);
}

void ScriptInterpTCL::linkVar(const String& name, const String& var) {
  Tcl_SetVar(interp, *name, "", TCL_GLOBAL_ONLY);
  Tcl_TraceVar(interp, *name, TCL_TRACE_READS | TCL_GLOBAL_ONLY, (Tcl_VarTraceProc *) this->TraceGetString, (ClientData) &var);
  Tcl_TraceVar(interp, *name, TCL_TRACE_WRITES | TCL_GLOBAL_ONLY, (Tcl_VarTraceProc *) this->TraceSetRO, (ClientData) NULL);
}

BDLIB_NS_END
