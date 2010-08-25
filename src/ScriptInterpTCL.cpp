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
  if (interp && !Tcl_InterpDeleted(interp)) {
    Tcl_DeleteInterp(interp);
    interp = NULL;
  }
  return 0;
}

String ScriptInterpTCL::eval(const String& script) {
  if (Tcl_EvalEx(interp, script.c_str(), script.length(), TCL_EVAL_GLOBAL) == TCL_OK) {
    //FIXME: Dry with TraceSetString
    Tcl_Obj* value = Tcl_GetObjResult(interp);
    int len = 0;
    char *cstr = Tcl_GetStringFromObj(value, &len);
    if (!cstr)
      //FIXME: Error handling
      return String();
    return String(cstr, len);
  } else
    return eval("set errorInfo");
  return String();
}

ScriptInterp::LoadError ScriptInterpTCL::loadScript(const String& fileName, String& resultStr) {
  if (fileName.rfind(".tcl", fileName.length() - 4) == String::npos)
    return SCRIPT_LOAD_WRONG_INTERP;
  if (Tcl_EvalFile(interp, *fileName) != TCL_OK) {
    //FIXME: Dry with TraceSetString
    Tcl_Obj* value = Tcl_GetObjResult(interp);
    int len = 0;
    char *cstr = Tcl_GetStringFromObj(value, &len);
    if (!cstr)
      resultStr = String();
    resultStr = String(cstr, len);
    return SCRIPT_LOAD_ERROR;
  }
  return SCRIPT_LOAD_OK;
}

int ScriptInterpTCL::tcl_callback_string(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
  script_cmd_handler_clientdata_t* ccd = (script_cmd_handler_clientdata_t*)clientData;
  ScriptArgsTCL args(objc, objv);
  String result = ((script_cmd_handler_string_t)ccd->callback)(*ccd->si, args, ccd->clientData);
  Tcl_SetObjResult(interp, (result.length() < INT_MAX) ? Tcl_NewStringObj(result.data(), result.length()) : NULL);
  return TCL_OK;
}

int ScriptInterpTCL::tcl_callback_int(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
  script_cmd_handler_clientdata_t* ccd = (script_cmd_handler_clientdata_t*)clientData;
  ScriptArgsTCL args(objc, objv);
  int result = ((script_cmd_handler_int_t)ccd->callback)(*ccd->si, args, ccd->clientData);
  Tcl_SetObjResult(interp, Tcl_NewIntObj(result));
  return TCL_OK;
}

int ScriptInterpTCL::tcl_callback_void(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
  script_cmd_handler_clientdata_t* ccd = (script_cmd_handler_clientdata_t*)clientData;
  ScriptArgsTCL args(objc, objv);
  ((script_cmd_handler_void_t)ccd->callback)(*ccd->si, args, ccd->clientData);
  return TCL_OK;
}

void ScriptInterpTCL::tcl_command_ondelete(ClientData clientData) {
  script_cmd_handler_clientdata_t* ccd = (script_cmd_handler_clientdata_t*)clientData;
  delete ccd;
}

void ScriptInterpTCL::createCommand(const String& name, script_cmd_handler_string_t callback, script_clientdata_t clientData) {
  script_cmd_handler_clientdata_t* ccd = new script_cmd_handler_clientdata_t(this, clientData, (script_cmd_handler_t) callback);
  Tcl_CreateObjCommand(interp, *name, tcl_callback_string, (ClientData*)ccd, tcl_command_ondelete);
}

void ScriptInterpTCL::createCommand(const String& name, script_cmd_handler_int_t callback, script_clientdata_t clientData) {
  script_cmd_handler_clientdata_t* ccd = new script_cmd_handler_clientdata_t(this, clientData, (script_cmd_handler_t) callback);
  Tcl_CreateObjCommand(interp, *name, tcl_callback_int, (ClientData*)ccd, tcl_command_ondelete);
}

void ScriptInterpTCL::createCommand(const String& name, script_cmd_handler_void_t callback, script_clientdata_t clientData) {
  script_cmd_handler_clientdata_t* ccd = new script_cmd_handler_clientdata_t(this, clientData, (script_cmd_handler_t) callback);
  Tcl_CreateObjCommand(interp, *name, tcl_callback_void, (ClientData*)ccd, tcl_command_ondelete);
}

void ScriptInterpTCL::setupTraces(const String& name, ClientData var, Tcl_VarTraceProc* get, Tcl_VarTraceProc* set) {
  Tcl_SetVar(interp, *name, "", TCL_GLOBAL_ONLY);
  Tcl_TraceVar(interp, *name, TCL_TRACE_READS | TCL_GLOBAL_ONLY, get, var);
  Tcl_TraceVar(interp, *name, TCL_TRACE_WRITES | TCL_GLOBAL_ONLY, set, var);
}

const char* ScriptInterpTCL::TraceSetRO (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  return "variable is read-only";
}

const char* ScriptInterpTCL::TraceGet (Tcl_Obj* value, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  if (value) {
    Tcl_SetVar2(interp,name1,name2,Tcl_GetStringFromObj(value,NULL), flags);
    Tcl_DecrRefCount(value);
  }
  return NULL;
}

Tcl_Obj* ScriptInterpTCL::TraceSet (Tcl_Interp *interp, char *name1, char *name2, int flags) {
  Tcl_Obj *name1o = Tcl_NewStringObj(name1,-1);
  Tcl_Obj *value = Tcl_ObjGetVar2(interp, name1o, 0, flags);
  Tcl_DecrRefCount(name1o);
  return value;
}

const char* ScriptInterpTCL::TraceGetString (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  String* str = (String*) clientData;
  Tcl_Obj *value = (str->length() < INT_MAX) ? Tcl_NewStringObj(str->data(), str->length()) : NULL;
  return TraceGet(value, interp, name1, name2, flags);
}

const char* ScriptInterpTCL::TraceSetString (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  Tcl_Obj *value = TraceSet(interp, name1, name2, flags);

  if (!value) goto fail;
  {
    int len = 0;
    char *cstr = Tcl_GetStringFromObj(value, &len);
    if (!cstr) {
//      SWIG_exception_fail(SWIG_ArgError(res), "in variable '""server_list""' of type '""char [256]""'");
      goto fail;
    }

    *(String*)clientData = String(cstr, len);
  }
  return NULL;
fail:
  return name1;
}

const char* ScriptInterpTCL::TraceGetInt (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  return TraceGet(Tcl_NewIntObj(*(int*)clientData), interp, name1, name2, flags);
}

const char* ScriptInterpTCL::TraceSetInt (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  Tcl_Obj *value = TraceSet(interp, name1, name2, flags);

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

const char* ScriptInterpTCL::TraceGetLong (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  return TraceGet(Tcl_NewLongObj(*(long*)clientData), interp, name1, name2, flags);
}

const char* ScriptInterpTCL::TraceSetLong (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  Tcl_Obj *value = TraceSet(interp, name1, name2, flags);

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

const char* ScriptInterpTCL::TraceGetDouble (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  return TraceGet(Tcl_NewDoubleObj(*(double*)clientData), interp, name1, name2, flags);
}

const char* ScriptInterpTCL::TraceSetDouble (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  Tcl_Obj *value = TraceSet(interp, name1, name2, flags);

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

const char* ScriptInterpTCL::TraceGetBool (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  return TraceGet(Tcl_NewBooleanObj(*(bool*)clientData), interp, name1, name2, flags);
}

const char* ScriptInterpTCL::TraceSetBool (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags) {
  Tcl_Obj *value = TraceSet(interp, name1, name2, flags);

  if (!value) goto fail;
  {
    int v;
    if (Tcl_GetBooleanFromObj(0,value, &v) != TCL_OK)
      return "Type Error";
    *(bool*)clientData = (bool)(v);
  }
  return NULL;
fail:
  return name1;
}

BDLIB_NS_END
