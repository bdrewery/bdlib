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
#include "Array.h"
#include "ScriptInterp.h"

#include <limits.h>
#include <sys/types.h>

#include <tcl.h>

BDLIB_NS_BEGIN

template <typename T>
struct c_to_tcl_cast;

#define c_to_tcl_castable(T)                                      \
template<>                                                        \
  struct c_to_tcl_cast<T>                                         \
  {                                                               \
    static Tcl_Obj* from(T value);                                \
  }

c_to_tcl_castable(int);
c_to_tcl_castable(unsigned int);
c_to_tcl_castable(long);
c_to_tcl_castable(unsigned long);
c_to_tcl_castable(double);
c_to_tcl_castable(bool);
c_to_tcl_castable(String);

template <typename T>
struct tcl_to_c_cast;

#define tcl_to_c_castable(T)                                      \
template<>                                                        \
  struct tcl_to_c_cast<T>                                         \
  {                                                               \
    static const char* from(Tcl_Obj* obj, T* value); \
  }

tcl_to_c_castable(int);
tcl_to_c_castable(unsigned int);
tcl_to_c_castable(long);
tcl_to_c_castable(unsigned long);
tcl_to_c_castable(double);
tcl_to_c_castable(bool);
tcl_to_c_castable(String);


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

    virtual String trigger(Array<String> params) const {
      Tcl_Obj* command = Tcl_DuplicateObj(obj);

      String result;
      if (Tcl_EvalObjEx(interp, command, TCL_EVAL_GLOBAL) == TCL_OK) {
        tcl_to_c_cast<String>::from(Tcl_GetObjResult(interp), &result);
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
      int value;
      tcl_to_c_cast<int>::from(my_objv[index], &value);
      return value;
    }

    virtual String getArgString(int index) const {
      if (size_t(index) >= length()) return String();
      String value;
      tcl_to_c_cast<String>::from(my_objv[index], &value);
      return value;
    }

    virtual ScriptCallbackTCL* getArgCallback(int index) const {
      if (size_t(index) >= length()) return NULL;
      ScriptCallbackTCL* callback = new ScriptCallbackTCL(my_objv[index], interp);
      return callback;
    }
};

#define define_tcl_traceGet(T)                                                                                                     \
template<>                                                                                                                         \
const char* tcl_traceGet<T> (ClientData clientData, Tcl_Interp* interp, char* name1, char* name2, int flags) {                     \
  return ScriptInterpTCL::TraceGet(c_to_tcl_cast<T>::from( *static_cast<T*>(clientData) ), interp, name1, name2, flags);           \
}

#define define_tcl_traceSet(T)                                                                                                     \
template<>                                                                                                                         \
const char* tcl_traceSet<T> (ClientData clientData, Tcl_Interp* interp, char* name1, char* name2, int flags) {                     \
  Tcl_Obj *obj = ScriptInterpTCL::TraceSet(interp, name1, name2, flags);                                                           \
  T value;                                                                                                                         \
  const char *error = NULL;                                                                                                        \
  if (!obj) goto fail;                                                                                                             \
                                                                                                                                   \
  error = tcl_to_c_cast<T>::from( obj , &value);                                                                                   \
  if (error) return "FIXME: ERROR DETECTED";                                                                                       \
  *static_cast<T*>(clientData) = value;                                                                                            \
  return NULL;                                                                                                                     \
fail:                                                                                                                              \
  return name1;                                                                                                                    \
}

template <typename T>
const char* tcl_traceGet (ClientData clientData, Tcl_Interp* interp, char* name1, char* name2, int flags);

template <typename T>
const char* tcl_traceSet (ClientData clientData, Tcl_Interp* interp, char* name1, char* name2, int flags);

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

        template <typename T>
          inline void linkVar(const String& name, T& var) {
            setupTraces(name, (ClientData) &var, (Tcl_VarTraceProc*) tcl_traceGet<T>, (Tcl_VarTraceProc*) tcl_traceSet<T>);
          };

        template <typename T>
          inline void linkVar(const String& name, const T& var) {
            setupTraces(name, (ClientData) &var, (Tcl_VarTraceProc*) tcl_traceGet<T>, (Tcl_VarTraceProc*) TraceSetRO);
          };
  private:
        static const char* TraceSetRO (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags);

  public:
        static Tcl_Obj* TraceSet (Tcl_Interp *interp, char *name1, char *name2, int flags);
        static const char* TraceGet (Tcl_Obj* value, Tcl_Interp *interp, char *name1, char *name2, int flags);
};

BDLIB_NS_END
#endif /* _BD_SCRIPTINTERPTCL_H */
