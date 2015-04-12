/* ScriptInterpTCL.cpp
 *
 */
#include "ScriptInterpTCL.h"

#ifdef USE_SCRIPT_TCL
#include <stdarg.h>
#include <algorithm> // min() / max()
#include <unordered_map>

BDLIB_NS_BEGIN

std::unordered_map<String, ScriptInterp::ScriptCmdPtr> ScriptInterpTCL::CmdHandlerData;
std::unordered_map<String, ScriptInterp::link_var_hook> ScriptInterpTCL::link_var_hooks;

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
    /* https://core.tcl.tk/tcl/tktview/35a839cba8a4e6bbdcd9567f958d56076fe5bd0f
    Tcl_Finalize();
    */
  }
  return 0;
}

inline String ScriptInterpTCL::eval(const String& script) {
  if (Tcl_EvalEx(interp, script.c_str(), script.length(), TCL_EVAL_GLOBAL) == TCL_OK) {
    Tcl_Obj* value = Tcl_GetObjResult(interp);
    return tcl_to_c_cast<String>::from(value, this);
  }
  return eval("set errorInfo");
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
int ScriptInterpTCL::ScriptCmdTCL::_createCommand_callback(
    ClientData clientData, Tcl_Interp *interp, int objc,
    Tcl_Obj *CONST objv[]) {
  String cmdName(tcl_to_c_cast<String>::from(objv[0], nullptr));
  ScriptCmdPtr ccd = CmdHandlerData[cmdName];

  if ((size_t(objc) - 1) < ccd->callbackParamMin || size_t(objc) - 1 > ccd->callbackParamMax) {
    String errorResult(String::printf("Wrong # args. Expected %zu, got %d.", ccd->callbackParamMin, objc - 1));
    if (ccd->usage)
      errorResult += String::printf(" Should be \"%s %s\"", cmdName.c_str(), ccd->usage);
    Tcl_SetObjResult(interp,
        c_to_tcl_cast<const String&>::from(errorResult, interp));
    return TCL_ERROR;
  }

  try {
    ccd->callback_proxy->call(objc, reinterpret_cast<void* CONST*>(objv), ccd->si, interp);
  } catch (BDLIB_NS::String& e) {
    Tcl_SetObjResult(interp,
        c_to_tcl_cast<const String&>::from(e, interp));
    return TCL_ERROR;
  } catch (...) {
    Tcl_SetObjResult(interp, c_to_tcl_cast<const char*>::from("Unhandled exception.", interp));
    return TCL_ERROR;
  }
  return TCL_OK;
}

void ScriptInterpTCL::setupTraces(const String& varName, ClientData var,
    Tcl_VarTraceProc* get, Tcl_VarTraceProc* set, link_var_hook hook_func) {
  Tcl_SetVar(interp, *varName, "", TCL_GLOBAL_ONLY);
  Tcl_TraceVar(interp, *varName, TCL_TRACE_READS | TCL_GLOBAL_ONLY, get, var);
  Tcl_TraceVar(interp, *varName, TCL_TRACE_WRITES | TCL_GLOBAL_ONLY, set, var);
  if (hook_func != nullptr)
    link_var_hooks[varName] = hook_func;
}

const char* ScriptInterpTCL::TraceSetRO(ClientData clientData,
    Tcl_Interp *interp, char *name1, char *name2, int flags) {
  return "variable is read-only";
}

const char* ScriptInterpTCL::TraceGet(Tcl_Obj* value, Tcl_Interp *interp,
    char *name1, char *name2, int flags) {
  if (value) {
    Tcl_SetVar2(interp,name1,name2, tcl_to_c_cast<const char*>::from(value, nullptr), flags);
    Tcl_DecrRefCount(value);
  }
  return nullptr;
}

Tcl_Obj* ScriptInterpTCL::TraceSet(Tcl_Interp *interp, char *name1,
    char *name2, int flags) {
  Tcl_Obj *name1o = c_to_tcl_cast<const char*>::from(name1, interp);
  Tcl_Obj *value = Tcl_ObjGetVar2(interp, name1o, 0, flags);
  Tcl_DecrRefCount(name1o);
  return value;
}

Tcl_Obj* c_to_tcl_cast<const Array<String>&>::from(const Array<String>& array, Tcl_Interp* interp) {
  Tcl_Obj* value = Tcl_NewListObj(array.length(), NULL);

  for (const auto& element : array) {
    Tcl_ListObjAppendElement(interp, value,
        c_to_tcl_cast<decltype(element)&>::from(element, interp));
  }

  return value;
}

Tcl_Obj* c_to_tcl_cast<const Array<Array<String>>&>::from(const Array<Array<String>>& array, Tcl_Interp* interp) {
  Tcl_Obj* value = Tcl_NewListObj(array.length(), NULL);

  for (const auto& element : array) {
    Tcl_ListObjAppendElement(interp, value,
        c_to_tcl_cast<decltype(element)&>::from(element, interp));
  }

  return value;
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

char* tcl_to_c_cast<char*>::from(Tcl_Obj* obj, ScriptInterp* si) {
  int len = 0;
  char *cstr = Tcl_GetStringFromObj(obj, &len);
  if (!cstr) {
    return (char*)"";
    //return "Type Error";
  }
  return cstr;
}

const char* tcl_to_c_cast<const char*>::from(Tcl_Obj* obj, ScriptInterp* si) {
  return tcl_to_c_cast<char*>::from(obj, si);
}

int8_t tcl_to_c_cast<int8_t>::from(Tcl_Obj* obj, ScriptInterp* si) {
  long v;
  if (Tcl_GetLongFromObj(0, obj, &v) == TCL_OK) {
    if ((v < INT8_MIN || v > INT8_MAX)) {
      //return "OverflowError";
      return 0;
    }
  } else {
    return 0;
    //return "Type Error";
  }
  return v;
}

uint8_t tcl_to_c_cast<uint8_t>::from(Tcl_Obj* obj, ScriptInterp* si) {
  long v;
  if (Tcl_GetLongFromObj(0, obj, &v) == TCL_OK) {
    if ((v < 0 || v > UINT8_MAX)) {
      //return "OverflowError";
      return 0;
    }
  } else {
    return 0;
    //return "Type Error";
  }
  return v;
}

int16_t tcl_to_c_cast<int16_t>::from(Tcl_Obj* obj, ScriptInterp* si) {
  long v;
  if (Tcl_GetLongFromObj(0, obj, &v) == TCL_OK) {
    if ((v < INT16_MIN || v > INT16_MAX)) {
      //return "OverflowError";
      return 0;
    }
  } else {
    return 0;
    //return "Type Error";
  }
  return v;
}

uint16_t tcl_to_c_cast<uint16_t>::from(Tcl_Obj* obj, ScriptInterp* si) {
  long v;
  if (Tcl_GetLongFromObj(0, obj, &v) == TCL_OK) {
    if ((v < 0 || v > UINT16_MAX)) {
      //return "OverflowError";
      return 0;
    }
  } else {
    return 0;
    //return "Type Error";
  }
  return v;
}

int32_t tcl_to_c_cast<int32_t>::from(Tcl_Obj* obj, ScriptInterp* si) {
  long v;
  if (Tcl_GetLongFromObj(0, obj, &v) == TCL_OK) {
    if ((v < INT32_MIN || v > INT32_MAX)) {
      //return "OverflowError";
      return 0;
    }
  } else {
    return 0;
    //return "Type Error";
  }
  return v;
}

uint32_t tcl_to_c_cast<uint32_t>::from(Tcl_Obj* obj, ScriptInterp* si) {
  long v;
  if (Tcl_GetLongFromObj(0, obj, &v) == TCL_OK) {
    if ((v < 0 || v > UINT32_MAX)) {
      //return "OverflowError";
      return 0;
    }
  } else {
    return 0;
    //return "Type Error";
  }
  return v;
}

int64_t tcl_to_c_cast<int64_t>::from(Tcl_Obj* obj, ScriptInterp* si) {
  long v;
  if (Tcl_GetLongFromObj(0, obj, &v) != TCL_OK) {
    return 0;
    //return "Type Error";
  }
  return v;
}

uint64_t tcl_to_c_cast<uint64_t>::from(Tcl_Obj* obj, ScriptInterp* si) {
  long v;
  if (Tcl_GetLongFromObj(0, obj, &v) == TCL_OK) {
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

ScriptCallbackerPtr tcl_to_c_cast<ScriptCallbackerPtr>::from(
    Tcl_Obj* obj, ScriptInterp* si) {
  int len = 0;
  char *cstr = Tcl_GetStringFromObj(obj, &len);
  if (!cstr) {
    return nullptr;
    //return "Type Error";
  }

  const String cmd(cstr, len);

  return std::make_shared<ScriptCallbackerTCL>(si, std::move(cmd));
}

Array<String> tcl_to_c_cast<Array<String>>::from(Tcl_Obj* obj, ScriptInterp* si) {
  int objc, i;
  Tcl_Obj** objv;

  // FIXME: Exception
  if (Tcl_ListObjGetElements((reinterpret_cast<ScriptInterpTCL*>(si))->interp, obj, &objc, &objv) != TCL_OK)
    return Array<String>();

  Array<String> ret(objc);

  for (i = 0; i < objc; ++i)
    ret << tcl_to_c_cast<String>::from(objv[i], si);

  return ret;
}

Array<Array<String>> tcl_to_c_cast<Array<Array<String>>>::from(Tcl_Obj* obj, ScriptInterp* si) {
  int objc, i;
  Tcl_Obj** objv;

  // FIXME: Exception
  if (Tcl_ListObjGetElements((reinterpret_cast<ScriptInterpTCL*>(si))->interp, obj, &objc, &objv) != TCL_OK)
    return Array<Array<String>>();

  Array<Array<String>> ret(objc);

  for (i = 0; i < objc; ++i)
    ret << tcl_to_c_cast<Array<String>>::from(objv[i], si);

  return ret;
}

BDLIB_NS_END
#endif /* USE_SCRIPT_TCL */
/* vim: set sts=2 sw=2 ts=8 et: */
