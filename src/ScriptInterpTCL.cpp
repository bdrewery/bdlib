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

const char* ScriptInterpTCL::TraceGetInt (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  Tcl_Obj *value = Tcl_NewIntObj(*(int*)clientData);
  if (value) {
    Tcl_SetVar2(interp,name1,name2,Tcl_GetStringFromObj(value,NULL), flags);
    Tcl_DecrRefCount(value);
  }
  return NULL;
}

const char* ScriptInterpTCL::TraceSetInt (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  Tcl_Obj *value = 0;
  Tcl_Obj *name1o = 0;

  name1o = Tcl_NewStringObj(name1,-1);
  value = Tcl_ObjGetVar2(interp, name1o, 0, flags);
  Tcl_DecrRefCount(name1o);
  if (!value) goto fail;
  {
    long v;
    if (Tcl_GetLongFromObj(0,value, &v) == TCL_OK) {
      if ((v < INT_MIN || v > INT_MAX))
        //SWIG_exception_fail(SWIG_ArgError(res), "in variable '""server_idx""' of type '""int""'");
        return "OverflowError";
    } else
      return "Type Error";
    *(int*)clientData = (int)(v);
  }
  return NULL;
fail:
  return name1;
}

void ScriptInterpTCL::linkVar(const String& name, int& var) {
  Tcl_SetVar(interp, *name, "", TCL_GLOBAL_ONLY);
  Tcl_TraceVar(interp, *name, TCL_TRACE_READS | TCL_GLOBAL_ONLY, (Tcl_VarTraceProc *) this->TraceGetInt, (ClientData) &var);
  Tcl_TraceVar(interp, *name, TCL_TRACE_WRITES | TCL_GLOBAL_ONLY, (Tcl_VarTraceProc *) this->TraceSetInt, (ClientData) &var);
}

void ScriptInterpTCL::linkVar(const String& name, const int& var) {
  Tcl_SetVar(interp, *name, "", TCL_GLOBAL_ONLY);
  Tcl_TraceVar(interp, *name, TCL_TRACE_READS | TCL_GLOBAL_ONLY, (Tcl_VarTraceProc *) this->TraceGetInt, (ClientData) &var);
  Tcl_TraceVar(interp, *name, TCL_TRACE_WRITES | TCL_GLOBAL_ONLY, (Tcl_VarTraceProc *) this->TraceSetRO, (ClientData) NULL);
}

const char* ScriptInterpTCL::TraceGetLong (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  Tcl_Obj *value = Tcl_NewLongObj(*(long*)clientData);
  if (value) {
    Tcl_SetVar2(interp,name1,name2,Tcl_GetStringFromObj(value,NULL), flags);
    Tcl_DecrRefCount(value);
  }
  return NULL;
}

const char* ScriptInterpTCL::TraceSetLong (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  Tcl_Obj *value = 0;
  Tcl_Obj *name1o = 0;

  name1o = Tcl_NewStringObj(name1,-1);
  value = Tcl_ObjGetVar2(interp, name1o, 0, flags);
  Tcl_DecrRefCount(name1o);
  if (!value) goto fail;
  {
    long v;
    if (Tcl_GetLongFromObj(0,value, &v) != TCL_OK)
      return "Type Error";
    *(long*)clientData = (long)(v);
  }
  return NULL;
fail:
  return name1;
}

void ScriptInterpTCL::linkVar(const String& name, long& var) {
  Tcl_SetVar(interp, *name, "", TCL_GLOBAL_ONLY);
  Tcl_TraceVar(interp, *name, TCL_TRACE_READS | TCL_GLOBAL_ONLY, (Tcl_VarTraceProc *) this->TraceGetLong, (ClientData) &var);
  Tcl_TraceVar(interp, *name, TCL_TRACE_WRITES | TCL_GLOBAL_ONLY, (Tcl_VarTraceProc *) this->TraceSetLong, (ClientData) &var);
}

void ScriptInterpTCL::linkVar(const String& name, const long& var) {
  Tcl_SetVar(interp, *name, "", TCL_GLOBAL_ONLY);
  Tcl_TraceVar(interp, *name, TCL_TRACE_READS | TCL_GLOBAL_ONLY, (Tcl_VarTraceProc *) this->TraceGetLong, (ClientData) &var);
  Tcl_TraceVar(interp, *name, TCL_TRACE_WRITES | TCL_GLOBAL_ONLY, (Tcl_VarTraceProc *) this->TraceSetRO, (ClientData) NULL);
}

const char* ScriptInterpTCL::TraceGetDouble (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  Tcl_Obj *value = Tcl_NewDoubleObj(*(double*)clientData);
  if (value) {
    Tcl_SetVar2(interp,name1,name2,Tcl_GetStringFromObj(value,NULL), flags);
    Tcl_DecrRefCount(value);
  }
  return NULL;
}

const char* ScriptInterpTCL::TraceSetDouble (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  Tcl_Obj *value = 0;
  Tcl_Obj *name1o = 0;

  name1o = Tcl_NewStringObj(name1,-1);
  value = Tcl_ObjGetVar2(interp, name1o, 0, flags);
  Tcl_DecrRefCount(name1o);
  if (!value) goto fail;
  {
    double v;
    if (Tcl_GetDoubleFromObj(0,value, &v) != TCL_OK)
      return "Type Error";
    *(double*)clientData = (double)(v);
  }
  return NULL;
fail:
  return name1;
}

void ScriptInterpTCL::linkVar(const String& name, double& var) {
  Tcl_SetVar(interp, *name, "", TCL_GLOBAL_ONLY);
  Tcl_TraceVar(interp, *name, TCL_TRACE_READS | TCL_GLOBAL_ONLY, (Tcl_VarTraceProc *) this->TraceGetDouble, (ClientData) &var);
  Tcl_TraceVar(interp, *name, TCL_TRACE_WRITES | TCL_GLOBAL_ONLY, (Tcl_VarTraceProc *) this->TraceSetDouble, (ClientData) &var);
}

void ScriptInterpTCL::linkVar(const String& name, const double& var) {
  Tcl_SetVar(interp, *name, "", TCL_GLOBAL_ONLY);
  Tcl_TraceVar(interp, *name, TCL_TRACE_READS | TCL_GLOBAL_ONLY, (Tcl_VarTraceProc *) this->TraceGetDouble, (ClientData) &var);
  Tcl_TraceVar(interp, *name, TCL_TRACE_WRITES | TCL_GLOBAL_ONLY, (Tcl_VarTraceProc *) this->TraceSetRO, (ClientData) NULL);
}

BDLIB_NS_END
