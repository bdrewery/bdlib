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
#include "HashTable.h"

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
c_to_tcl_castable(const char*);

template <typename T>
struct tcl_to_c_cast;

#define tcl_to_c_castable(T)                                      \
template<>                                                        \
  struct tcl_to_c_cast<T>                                         \
  {                                                               \
    static T from(Tcl_Obj* obj, ScriptInterp* si); \
  }

tcl_to_c_castable(int);
tcl_to_c_castable(unsigned int);
tcl_to_c_castable(long);
tcl_to_c_castable(unsigned long);
tcl_to_c_castable(double);
tcl_to_c_castable(bool);
tcl_to_c_castable(String);
tcl_to_c_castable(const char*);
tcl_to_c_castable(ScriptCallbacker*);

#define define_tcl_traceGet(T)                                                                                                     \
template<>                                                                                                                         \
const char* tcl_traceGet<T> (ClientData clientData, Tcl_Interp* interp, char* name1, char* name2, int flags) {                     \
  return ScriptInterpTCL::TraceGet(c_to_tcl_cast<T>::from( *static_cast<T*>(clientData) ), interp, name1, name2, flags);           \
}

#define define_tcl_traceSet(T)                                                                                                     \
template<>                                                                                                                         \
const char* tcl_traceSet<T> (ClientData clientData, Tcl_Interp* interp, char* name1, char* name2, int flags) {                     \
  Tcl_Obj *obj = ScriptInterpTCL::TraceSet(interp, name1, name2, flags);                                                           \
  if (!obj) return name1;                                                                                                          \
  const T oldval(*static_cast<T*>(clientData));                                                                                    \
                                                                                                                                   \
  *static_cast<T*>(clientData) = std::move(tcl_to_c_cast<T>::from(obj, NULL));                                                     \
  if (ScriptInterpTCL::link_var_hooks[name1])                                                                                      \
    (ScriptInterpTCL::link_var_hooks[name1])((const void*)&oldval, (const void*)(clientData));                                     \
  return NULL;                                                                                                                     \
}

class ScriptCallbackTCLBase : public ScriptCallbackBase {
  public:
    virtual ~ScriptCallbackTCLBase() {};
};

#include "ScriptInterpTCLCallbacks.h"

template <typename T>
const char* tcl_traceGet (ClientData clientData, Tcl_Interp* interp, char* name1, char* name2, int flags);

template <typename T>
const char* tcl_traceSet (ClientData clientData, Tcl_Interp* interp, char* name1, char* name2, int flags);

class ScriptCallbackerTCL : public ScriptCallbacker {
  private:
    ScriptCallbackerTCL(const ScriptCallbackerTCL&) = delete;
    ScriptCallbackerTCL& operator=(const ScriptCallbackerTCL&) = delete;
  public:
    ScriptCallbackerTCL(ScriptInterp* _si, const String _cmd) : ScriptCallbacker(_si, _cmd) {};
    virtual ~ScriptCallbackerTCL() {};
    virtual String call(const Array<String>& params);
};

class ScriptInterpTCL : public ScriptInterp {
  private:
        Tcl_Interp *interp;
        static HashTable<String, script_cmd_handler_clientdata*> CmdHandlerData;

        ScriptInterpTCL(const ScriptInterpTCL&) = delete;
        ScriptInterpTCL& operator=(const ScriptInterpTCL&) = delete;

        void setupTraces(const String& name, ClientData var, Tcl_VarTraceProc* get, Tcl_VarTraceProc* set, link_var_hook hook_func);
        static int _createCommand_callback(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);

        void _createCommand(const String& cmdName, ScriptCallbackBase* callback_proxy, size_t callbackParamMin, size_t callbackParamMax) {
          script_cmd_handler_clientdata* ccd = new script_cmd_handler_clientdata(this, callback_proxy, callbackParamMin, callbackParamMax);
          CmdHandlerData[cmdName] = ccd;
          Tcl_CreateObjCommand(interp, *cmdName, _createCommand_callback, NULL, NULL);
        }

  protected:
        virtual int init();
        virtual int destroy();

  public:
        static HashTable<String, link_var_hook> link_var_hooks;
        ScriptInterpTCL() : ScriptInterp(), interp(NULL) {init();};
        virtual ~ScriptInterpTCL() {
          // Delete all of my ccd
          CmdHandlerData.each([](String cmdName, script_cmd_handler_clientdata* ccd, void* param) {
              delete ccd->callback_proxy;
              delete ccd;
          });
          CmdHandlerData.clear();
          destroy();
        };

        virtual String eval(const String& script);
        virtual LoadError loadScript(const String& fileName, String& resultStr);

        template<typename ReturnType, typename... Params>
        inline void createCommand(const String& cmdName, ReturnType(*callback)(Params...), size_t min_params = size_t(-1)) {
          _createCommand(cmdName, new ScriptCallbackTCL<ReturnType, Params...>(callback), min_params == size_t(-1) ? sizeof...(Params) : min_params, sizeof...(Params));
        }

        virtual void deleteCommand(const String& cmdName) {
          script_cmd_handler_clientdata* ccd = CmdHandlerData[cmdName];
          delete ccd->callback_proxy;
          delete ccd;
          CmdHandlerData.remove(cmdName);
          Tcl_DeleteCommand(interp, *cmdName);
        }

        /* Variable linking */

        /**
         * @brief Link a C variable to the interp
         * @param varName The name to create the variable as
         * @param var The variable to link to
         */
        template <typename T>
          inline void linkVar(const String& varName, T& var, link_var_hook hook_func = NULL) {
            setupTraces(varName, (ClientData) &var, (Tcl_VarTraceProc*) tcl_traceGet<T>, (Tcl_VarTraceProc*) tcl_traceSet<T>, hook_func);
          };

        /**
         * @brief Link a const C variable to the interp
         * @param varName The name to create the variable as
         * @param var The variable to link to
         * @note The variable will be created as read-only
         */
        template <typename T>
          inline void linkVar(const String& varName, const T& var, link_var_hook hook_func = NULL) {
            setupTraces(varName, (ClientData) &var, (Tcl_VarTraceProc*) tcl_traceGet<T>, (Tcl_VarTraceProc*) TraceSetRO, hook_func);
          };

        /**
         * @brief Remove a variable from the interp
         * @param varName The name of the variable to remove
         */
        virtual void unlinkVar(const String& varName) {
          Tcl_UnsetVar(interp, *varName, TCL_GLOBAL_ONLY);
          link_var_hooks.remove(varName);
        }

        virtual script_type type() const { return SCRIPT_TYPE_TCL; }
  private:
        static const char* TraceSetRO (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags);

  public:
        static Tcl_Obj* TraceSet (Tcl_Interp *interp, char *name1, char *name2, int flags);
        static const char* TraceGet (Tcl_Obj* value, Tcl_Interp *interp, char *name1, char *name2, int flags);
};

BDLIB_NS_END
#endif /* _BD_SCRIPTINTERPTCL_H */
