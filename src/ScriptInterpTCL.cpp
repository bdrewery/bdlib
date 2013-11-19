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

HashTable<String, ScriptInterpTCL::script_cmd_handler_clientdata*> ScriptInterpTCL::CmdHandlerData;
HashTable<String, ScriptInterp::link_var_hook> ScriptInterpTCL::link_var_hooks;

/* Define static tcl_traceGet() template functions */
define_tcl_traceGet(int);
define_tcl_traceGet(unsigned int);
define_tcl_traceGet(long);
define_tcl_traceGet(unsigned long);
define_tcl_traceGet(double);
define_tcl_traceGet(bool);
define_tcl_traceGet(String);

define_tcl_traceSet(int);
//define_tcl_traceSet(unsigned int);
define_tcl_traceSet(long);
//define_tcl_traceSet(unsigned long);
define_tcl_traceSet(double);
define_tcl_traceSet(bool);
define_tcl_traceSet(String);

String ScriptCallbackerTCL::call(const Array<String>& params) {
  return si->eval(String::printf("%s %s", cmd.c_str(), params.join(" ", true).c_str()));
}

int ScriptInterpTCL::init() {
  // create interp
  interp = Tcl_CreateInterp();
  //Tcl_FindExecutable(binname);

  if (Tcl_Init(interp) != TCL_OK) {
    Tcl_Obj* result = Tcl_GetObjResult(interp);
    fprintf(stderr, "Tcl_Init error: %s\n", tcl_to_c_cast<const char*>::from(result, this));
    return 1;
  }
  return 0;
}

int ScriptInterpTCL::destroy() {
  if (interp && !Tcl_InterpDeleted(interp)) {
    Tcl_DeleteInterp(interp);
    interp = nullptr;
  }
  return 0;
}

String ScriptInterpTCL::eval(const String& script) {
  if (Tcl_EvalEx(interp, script.c_str(), script.length(), TCL_EVAL_GLOBAL) == TCL_OK) {
    Tcl_Obj* value = Tcl_GetObjResult(interp);
    return tcl_to_c_cast<String>::from(value, this);
  } else
    return eval("set errorInfo");
  return String();
}

ScriptInterp::LoadError ScriptInterpTCL::loadScript(const String& fileName, String& resultStr) {
  if (fileName.rfind(".tcl", fileName.length() - 4) == String::npos)
    return SCRIPT_LOAD_WRONG_INTERP;
  if (Tcl_EvalFile(interp, *fileName) != TCL_OK) {
    Tcl_Obj* value = Tcl_GetObjResult(interp);
    resultStr = tcl_to_c_cast<String>::from(value, this);
    return SCRIPT_LOAD_ERROR;
  }
  return SCRIPT_LOAD_OK;
}

/**
 * @brief Callback handler for when a TCL->C function call is made
 */
int ScriptInterpTCL::_createCommand_callback(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
  String cmdName(tcl_to_c_cast<String>::from(objv[0], nullptr));
  script_cmd_handler_clientdata* ccd = CmdHandlerData[cmdName];

  if ((size_t(objc) - 1) < ccd->callbackParamMin || size_t(objc) - 1 > ccd->callbackParamMax) {
    String errorResult(String::printf("Wrong # args. Expected %zu, got %d", ccd->callbackParamMin, objc - 1));
    Tcl_SetObjResult(interp, c_to_tcl_cast<String>::from(errorResult));
    return TCL_ERROR;
  }

  ccd->callback_proxy->call(objc, reinterpret_cast<void* CONST*>(objv), ccd->si, interp);
  return TCL_OK;
}

void ScriptInterpTCL::setupTraces(const String& varName, ClientData var, Tcl_VarTraceProc* get, Tcl_VarTraceProc* set, link_var_hook hook_func) {
  Tcl_SetVar(interp, *varName, "", TCL_GLOBAL_ONLY);
  Tcl_TraceVar(interp, *varName, TCL_TRACE_READS | TCL_GLOBAL_ONLY, get, var);
  Tcl_TraceVar(interp, *varName, TCL_TRACE_WRITES | TCL_GLOBAL_ONLY, set, var);
  link_var_hooks[varName] = hook_func;
}

const char* ScriptInterpTCL::TraceSetRO (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  return "variable is read-only";
}

const char* ScriptInterpTCL::TraceGet (Tcl_Obj* value, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  if (value) {
    Tcl_SetVar2(interp,name1,name2, tcl_to_c_cast<const char*>::from(value, nullptr), flags);
    Tcl_DecrRefCount(value);
  }
  return nullptr;
}

Tcl_Obj* ScriptInterpTCL::TraceSet (Tcl_Interp *interp, char *name1, char *name2, int flags) {
  Tcl_Obj *name1o = c_to_tcl_cast<const char*>::from(name1);
  Tcl_Obj *value = Tcl_ObjGetVar2(interp, name1o, 0, flags);
  Tcl_DecrRefCount(name1o);
  return value;
}

Tcl_Obj* c_to_tcl_cast<int>::from(int value) {
  return Tcl_NewIntObj(value);
}

Tcl_Obj* c_to_tcl_cast<unsigned int>::from(unsigned int value) {return c_to_tcl_cast<int>::from(value);}

Tcl_Obj* c_to_tcl_cast<long>::from(long value) {
  return Tcl_NewLongObj(value);
}

Tcl_Obj* c_to_tcl_cast<unsigned long>::from(unsigned long value) {return c_to_tcl_cast<long>::from(value);}

Tcl_Obj* c_to_tcl_cast<double>::from(double value) {
  return Tcl_NewDoubleObj(value);
}

Tcl_Obj* c_to_tcl_cast<String>::from(String value) {
  return (value.length() < INT_MAX) ? Tcl_NewStringObj(value.data(), value.length()) : nullptr;
}

Tcl_Obj* c_to_tcl_cast<const char *>::from(const char* value) {return c_to_tcl_cast<String>::from(value);}

Tcl_Obj* c_to_tcl_cast<bool>::from(bool value) {
  return Tcl_NewBooleanObj(value);
}

/* tcl->c casting */
String tcl_to_c_cast<String>::from(Tcl_Obj* obj, ScriptInterp* si) {
  int len = 0;
  char *cstr = Tcl_GetStringFromObj(obj, &len);
  if (!cstr) {
    return String();
    //return "Type Error";
  }

  return String(cstr, len);
  //return nullptr;
}

const char* tcl_to_c_cast<const char*>::from(Tcl_Obj* obj, ScriptInterp* si) {
  int len = 0;
  char *cstr = Tcl_GetStringFromObj(obj, &len);
  if (!cstr) {
    return "";
    //return "Type Error";
  }
  return cstr;
}


int tcl_to_c_cast<int>::from(Tcl_Obj* obj, ScriptInterp* si) {
  long v;
  if (Tcl_GetLongFromObj(0, obj, &v) == TCL_OK) {
    if ((v < INT_MIN || v > INT_MAX)) {
      //return "OverflowError";
      return 0;
    }
  } else {
    return 0;
    //return "Type Error";
  }
  return v;
}

long tcl_to_c_cast<long>::from(Tcl_Obj* obj, ScriptInterp* si) {
  long v;
  if (Tcl_GetLongFromObj(0, obj, &v) != TCL_OK) {
    return 0;
    //return "Type Error";
  }
  return v;
}

double tcl_to_c_cast<double>::from(Tcl_Obj* obj, ScriptInterp* si) {
  double v;
  if (Tcl_GetDoubleFromObj(0, obj, &v) != TCL_OK) {
    return 0;
    //return "Type Error";
  }
  return v;
}

bool tcl_to_c_cast<bool>::from(Tcl_Obj* obj, ScriptInterp* si) {
  int v;
  if (Tcl_GetBooleanFromObj(0, obj, &v) != TCL_OK) {
    return false;
    //return "Type Error";
  }
  return v ? true : false;
}

ScriptCallbacker* tcl_to_c_cast<ScriptCallbacker*>::from(Tcl_Obj* obj, ScriptInterp* si) {
  int len = 0;
  char *cstr = Tcl_GetStringFromObj(obj, &len);
  if (!cstr) {
    return nullptr;
    //return "Type Error";
  }

  const String cmd(cstr, len);

  ScriptCallbackerTCL* scb = new ScriptCallbackerTCL(si, cmd);

  return scb;
}

BDLIB_NS_END
