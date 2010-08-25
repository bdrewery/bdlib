/* ScriptInterpTCL.h
 *
 * Copyright (C) Bryan Drewery
 *
 * This program is private and may not be distributed, modified
 * or used without express permission of Bryan Drewery.
 *
 * THIS PROGRAM IS DISTRIBUTED WITHOUT ANY WARRANTY.
 * IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY
 * FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
 * DERIVATIVES THEREOF, EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE
 * IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE
 * NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
 * MODIFICATIONS.
 *
 */
#ifndef _BD_SCRIPTINTERPTCL_H
#define _BD_SCRIPTINTERPTCL_H 1

#include "bdlib.h"
#include "String.h"
#include "ScriptInterp.h"

#include <limits.h>
#include <sys/types.h>

#include <tcl.h>

BDLIB_NS_BEGIN

class ScriptCallbackTCL : public ScriptCallback {
  private:
    Tcl_Obj* obj;
    Tcl_Interp* interp;

    // Don't allow copying
    ScriptCallbackTCL(const ScriptCallbackTCL&) : ScriptCallback(), obj(NULL), interp(NULL) {};
    ScriptCallbackTCL& operator=(const ScriptCallbackTCL&) {return *this;};
  public:
    ScriptCallbackTCL() : ScriptCallback(), obj(NULL), interp(NULL) {};
    ScriptCallbackTCL(Tcl_Obj* _obj, Tcl_Interp* _interp) : ScriptCallback(), obj(_obj), interp(_interp) {
      Tcl_IncrRefCount(obj);
    };
    virtual ~ScriptCallbackTCL() {
      Tcl_DecrRefCount(obj);
    };

    virtual String trigger(...) const {
      Tcl_Obj* command = Tcl_DuplicateObj(obj);

      String result;
      if (Tcl_EvalObjEx(interp, command, TCL_EVAL_GLOBAL) == TCL_OK) {
        //FIXME: Dry with TraceSetString
        Tcl_Obj* value = Tcl_GetObjResult(interp);
        int len = 0;
        char *cstr = Tcl_GetStringFromObj(value, &len);
        if (!cstr)
          //FIXME: Error handling
          result = String();
        else
          result = String(cstr, len);
      } else
        Tcl_BackgroundError(interp);

      /* Clear any errors or stray messages. */
      Tcl_ResetResult(interp);
      return result;
    }


    virtual size_t hash() const { return (size_t)obj; };
};

template<typename T>
  struct Hash;

template<>
  struct Hash<ScriptCallbackTCL>
    {
          inline size_t operator()(const ScriptCallbackTCL& val) const { return val.hash(); }
    };

class ScriptArgsTCL : public ScriptArgs {
  private:
    Tcl_Obj** my_objv;
    Tcl_Interp* interp;

    // Don't allow copying
    ScriptArgsTCL(const ScriptArgsTCL&) : ScriptArgs(), my_objv(), interp(NULL) {};
    ScriptArgsTCL& operator=(const ScriptArgsTCL&) {return *this;};
  public:
    ScriptArgsTCL() : ScriptArgs(), my_objv(), interp(NULL) {};
    ScriptArgsTCL(int objc, Tcl_Obj* CONST objv[], Tcl_Interp* _interp) : ScriptArgs(objc), my_objv(), interp(_interp) {
      my_objv = new Tcl_Obj*[objc];
      for (size_t i = 0; i < argc; ++i) {
        my_objv[i] = objv[i];
        Tcl_IncrRefCount(my_objv[i]);
      }
    }
    virtual ~ScriptArgsTCL() {
      for (size_t i = 0; i < argc; ++i)
        Tcl_DecrRefCount(my_objv[i]);
      delete[] my_objv;
    }

    virtual int getArgInt(int index) const {
      if (size_t(index) >= length()) return 0;
      long v = 0;
      //FIXME: DRY with TraceSetInt
      if (Tcl_GetLongFromObj(0, my_objv[index], &v) == TCL_OK) {
        if ((v < INT_MIN || v > INT_MAX))
          //FIXME error - overflow
          return 0;
      } else
        // FIXME - error getting int - wrong type?
        return 0;
      return (int)v;
    }

    virtual String getArgString(int index) const {
      if (size_t(index) >= length()) return String();
      //FIXME: DRY with TraceSetString
      int len = 0;
      char *cstr = Tcl_GetStringFromObj(my_objv[index], &len);
      if (!cstr) return String();
      return String(cstr, len);
    }

    virtual ScriptCallbackTCL* getArgCallback(int index) const {
      if (size_t(index) >= length()) return NULL;
      ScriptCallbackTCL* callback = new ScriptCallbackTCL(my_objv[index], interp);
      return callback;
    }
};

class ScriptInterpTCL : public ScriptInterp {
  private:
        Tcl_Interp *interp;

        // Don't allow copying
        ScriptInterpTCL(const ScriptInterpTCL&) : ScriptInterp(), interp(NULL) {};
        ScriptInterpTCL& operator=(const ScriptInterpTCL&) {return *this;};

        void setupTraces(const String& name, ClientData var, Tcl_VarTraceProc* get, Tcl_VarTraceProc* set);
        static int tcl_callback_string(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
        static int tcl_callback_int(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
        static int tcl_callback_void(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
        static void tcl_command_ondelete(ClientData clientData);
  protected:
        virtual int init();
        virtual int destroy();

  public:
        ScriptInterpTCL() : ScriptInterp(), interp(NULL) {init();};
        virtual ~ScriptInterpTCL() {destroy();};

        virtual String eval(const String& script);
        virtual LoadError loadScript(const String& fileName, String& resultStr);

        virtual void createCommand(const String& name, script_cmd_handler_string_t callback, script_clientdata_t clientData = NULL);
        virtual void createCommand(const String& name, script_cmd_handler_int_t callback, script_clientdata_t clientData = NULL);
        virtual void createCommand(const String& name, script_cmd_handler_void_t callback, script_clientdata_t clientData = NULL);

        virtual void deleteCommand(const String& name) {
          Tcl_DeleteCommand(interp, *name);
        }

        /* Variable linking */

#define LINK_VAR(_type, _GET, _SET) \
        virtual inline void linkVar(const String& name, _type& var) { setupTraces(name, (ClientData) &var, (Tcl_VarTraceProc*) _GET, (Tcl_VarTraceProc*) _SET); }; \
        virtual inline void linkVar(const String& name, const _type& var) { setupTraces(name, (ClientData) &var, (Tcl_VarTraceProc*) _GET, (Tcl_VarTraceProc*) TraceSetRO); }

        LINK_VAR(String, TraceGetString, TraceSetString);
        LINK_VAR(int, TraceGetInt, TraceSetInt);
        LINK_VAR(unsigned int, TraceGetInt, TraceSetInt);
        LINK_VAR(long, TraceGetLong, TraceSetLong);
        LINK_VAR(unsigned long, TraceGetLong, TraceSetLong);
        LINK_VAR(double, TraceGetDouble, TraceSetDouble);
        LINK_VAR(bool, TraceGetBool, TraceSetBool);
#undef LINK_VAR

  private:
#define TRACE_PROTO(_name) static const char* _name (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags)
        TRACE_PROTO(TraceSetRO);
        TRACE_PROTO(TraceGetString);
        TRACE_PROTO(TraceSetString);
        TRACE_PROTO(TraceGetInt);
        TRACE_PROTO(TraceSetInt);
        TRACE_PROTO(TraceGetLong);
        TRACE_PROTO(TraceSetLong);
        TRACE_PROTO(TraceGetDouble);
        TRACE_PROTO(TraceSetDouble);
        TRACE_PROTO(TraceGetBool);
        TRACE_PROTO(TraceSetBool);
#undef TRACE_PROTO
        static const char* TraceGet (Tcl_Obj* value, Tcl_Interp *interp, char *name1, char *name2, int flags);
        static Tcl_Obj* TraceSet (Tcl_Interp *interp, char *name1, char *name2, int flags);
};


BDLIB_NS_END
#endif /* _BD_SCRIPTINTERPTCL_H */
