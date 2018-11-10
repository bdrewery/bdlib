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

#ifdef USE_SCRIPT_TCL
#include "String.h"
#include "ScriptInterp.h"

#include <unordered_map>
#include <memory>
#include "make_unique.h"
#include <type_traits>
#include <cstddef>
#include <cstdint>
#include <limits.h>
#include <sys/types.h>

#include <tcl.h>

BDLIB_NS_BEGIN

template <typename T>
struct c_to_tcl_cast;

#define c_to_tcl_castable(T)                                                  \
template<>                                                                    \
  struct c_to_tcl_cast<const T&>                                              \
  {                                                                           \
    static Tcl_Obj* from(const T& value, Tcl_Interp*);                        \
  }

#define c_to_tcl_castable_ptr(T)                                              \
template<>                                                                    \
  struct c_to_tcl_cast<const T>                                               \
  {                                                                           \
    static Tcl_Obj* from(const T value, Tcl_Interp*);                         \
  }

c_to_tcl_castable(int8_t);
c_to_tcl_castable(uint8_t);
c_to_tcl_castable(int16_t);
c_to_tcl_castable(uint16_t);
c_to_tcl_castable(int32_t);
c_to_tcl_castable(uint32_t);
c_to_tcl_castable(int64_t);
c_to_tcl_castable(uint64_t);
c_to_tcl_castable(double);
c_to_tcl_castable(bool);
c_to_tcl_castable_ptr(char*);
c_to_tcl_castable(String);
c_to_tcl_castable(Array<String>);
c_to_tcl_castable(Array<Array<String>>);

template <typename T>
struct tcl_to_c_cast;

#define tcl_to_c_castable(T)                                      \
template<>                                                        \
  struct tcl_to_c_cast<T>                                         \
  {                                                               \
    static T from(Tcl_Obj* obj, ScriptInterp* si);                \
  }

#define tcl_to_c_castable_pure(T)                                 \
template<>                                                        \
  struct tcl_to_c_cast<T>                                         \
  {                                                               \
    static T from(Tcl_Obj* obj, ScriptInterp* si)                 \
      __attribute__((pure));                                      \
  }

tcl_to_c_castable_pure(int8_t);
tcl_to_c_castable_pure(uint8_t);
tcl_to_c_castable_pure(int16_t);
tcl_to_c_castable_pure(uint16_t);
tcl_to_c_castable_pure(int32_t);
tcl_to_c_castable_pure(uint32_t);
tcl_to_c_castable_pure(int64_t);
tcl_to_c_castable_pure(uint64_t);
tcl_to_c_castable_pure(double);
tcl_to_c_castable_pure(bool);
tcl_to_c_castable(char*);
tcl_to_c_castable(String);
/* Only used internally, no tcl_traceSet defined for these. */
tcl_to_c_castable(const char*);
tcl_to_c_castable(ScriptCallbackerPtr);
tcl_to_c_castable(Array<String>);
tcl_to_c_castable(Array<Array<String>>);

class ScriptCommandHandlerTCLBase : public ScriptCommandHandlerBase {
  public:
    virtual ~ScriptCommandHandlerTCLBase() {};
};

#include "ScriptInterpTCLCallbacks.h"

class ScriptCallbackerTCL : public ScriptCallbacker {
  private:
    ScriptCallbackerTCL(const ScriptCallbackerTCL&) = delete;
    ScriptCallbackerTCL& operator=(const ScriptCallbackerTCL&) = delete;
  public:
    ScriptCallbackerTCL(ScriptInterp* _si, const String& _cmd) :
      ScriptCallbacker(_si, _cmd) {};
    virtual ~ScriptCallbackerTCL() {};
    inline virtual String call(const Array<String>& params = Array<String>()) {
      return si->eval(String::printf("%s %s", cmd.c_str(),
            params.join(" ", true).c_str()));
    }

};

class ScriptInterpTCL : public ScriptInterp {
  friend Array<String> tcl_to_c_cast<Array<String>>::from(Tcl_Obj* obj,
      ScriptInterp* si);
  friend Array<Array<String>> tcl_to_c_cast<Array<Array<String>>>::from(
      Tcl_Obj* obj, ScriptInterp* si);
  private:
        Tcl_Interp *interp;
        static std::unordered_map<String, ScriptCmdPtr>
          CmdHandlerData;

        ScriptInterpTCL(const ScriptInterpTCL&) = delete;
        ScriptInterpTCL& operator=(const ScriptInterpTCL&) = delete;

        struct ScriptCmdTCL : public ScriptCmd {
          static int _createCommand_callback(ClientData clientData,
              Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);

          ScriptCmdTCL(const ScriptCmdTCL&) = delete;
          ScriptCmdTCL& operator=(const ScriptCmdTCL&) = delete;
          ScriptCmdTCL(ScriptInterpTCL* _si, const String &_cmdName,
              std::unique_ptr<ScriptCommandHandlerBase> _callback_proxy,
              const char* _usage, size_t _callbackParamMin,
              size_t _callbackParamMax) :
              ScriptCmd(_si, std::move(_cmdName), std::move(_callback_proxy),
                  _usage, _callbackParamMin, _callbackParamMax) {};
          virtual ~ScriptCmdTCL() {
            this->unregisterCmd();
          }
          virtual void registerCmd() {
            if (registered)
              return;
            Tcl_CreateObjCommand(static_cast<ScriptInterpTCL*>(si)->interp,
                *cmdName, _createCommand_callback, nullptr, nullptr);
            ScriptCmd::registerCmd();
          }
          virtual void unregisterCmd() {
            if (!registered)
              return;
            Tcl_DeleteCommand(static_cast<ScriptInterpTCL*>(si)->interp,
                *cmdName);
            ScriptCmd::unregisterCmd();
          }
        };

        void _createCommand(const String& cmdName,
            std::unique_ptr<ScriptCommandHandlerBase> callback_proxy,
            const char* usage,
            size_t callbackParamMin, size_t callbackParamMax) {
          auto ccd = std::make_shared<ScriptCmdTCL>(this, cmdName,
              std::move(callback_proxy), usage, callbackParamMin,
              callbackParamMax);
          CmdHandlerData[cmdName] = ccd;
          ccd->registerCmd();
        }

        struct trace_ptr_data {
          void* ptr;
          size_t size;
          trace_ptr_data(void* _ptr, size_t _size) : ptr(_ptr), size(_size) {};
          trace_ptr_data(const trace_ptr_data&) = delete;
          trace_ptr_data& operator=(const trace_ptr_data&) = delete;
        };
        std::unordered_map<String, std::unique_ptr<trace_ptr_data>> trace_ptrs;

        void setupTraces(const String& name, ClientData var,
            Tcl_VarTraceProc* get, Tcl_VarTraceProc* set,
            link_var_hook_t hook_func);

        static const char* TraceGet (Tcl_Obj* value, Tcl_Interp *interp,
            char *name1, char *name2, int flags);

        template<typename T>
        static inline const char* tcl_traceGet(ClientData clientData,
            Tcl_Interp* interp, char* name1, char* name2, int flags) {
          return TraceGet(
              c_to_tcl_cast<T&>::from(*static_cast<T*>(clientData), interp),
              interp, name1, name2, flags);
        }
        template<typename T>
        static inline const char* tcl_traceGetPtrData(ClientData clientData,
            Tcl_Interp* interp, char* name1, char* name2, int flags) {
          auto data = static_cast<const trace_ptr_data*>(clientData);
          return TraceGet(
              c_to_tcl_cast<T>::from(static_cast<T>(data->ptr),
                interp), interp, name1, name2, flags);
        }
        template<typename T>
        static inline const char* tcl_traceGetPtr(ClientData clientData,
            Tcl_Interp* interp, char* name1, char* name2, int flags) {
          return TraceGet(
              c_to_tcl_cast<T>::from(static_cast<T>(clientData),
                interp), interp, name1, name2, flags);
        }

        template<typename T>
        static const char* tcl_traceSet(ClientData clientData,
            Tcl_Interp* interp, char* name1, char* name2, int flags) {
          Tcl_Obj *obj = ScriptInterpTCL::TraceSet(interp, name1, name2,
              flags);
          if (!obj)
            return name1;
          const auto oldval(std::move(*static_cast<T*>(clientData)));

          *static_cast<T*>(clientData) =
            std::move(tcl_to_c_cast<T>::from(obj, nullptr));
          const auto link_var_hook = link_var_hooks.find(name1);
          if (link_var_hook != std::end(link_var_hooks))
            link_var_hook->second(
                (const void*)&oldval, (const void*)(clientData));
          return nullptr;
        }

        template<typename T>
        static const char* tcl_traceSetPtr(ClientData clientData,
            Tcl_Interp* interp, char* name1, char* name2, int flags) {
          Tcl_Obj *obj = ScriptInterpTCL::TraceSet(interp, name1, name2,
              flags);
          if (!obj)
            return name1;
          const auto link_var_hook = link_var_hooks.find(name1);
          auto data = static_cast<const trace_ptr_data*>(clientData);
          /* Save oldval */
          typedef typename std::pointer_traits<T>::element_type T_name;
          std::unique_ptr<T_name[]> oldval;
          if (link_var_hook != std::end(link_var_hooks)) {
            oldval = std::move(std::make_unique<T_name[]>(data->size));
            memcpy(oldval.get(), static_cast<T>(data->ptr), data->size);
          }

          memmove(data->ptr, tcl_to_c_cast<T>::from(obj, nullptr),
              data->size);
          if __CPP17_IFCONSTEXPR (std::is_same<T, char*>::value ||
              std::is_same<T, const char*>::value)
            static_cast<T>(data->ptr)[data->size - 1] = '\0';
          if (link_var_hook != std::end(link_var_hooks))
            link_var_hook->second(
                (const void*)(oldval.get()), (const void*)(data->ptr));
          return nullptr;
        }

        static const char* TraceSetRO (const ClientData clientData,
            const Tcl_Interp *interp, const char *name1, const char *name2,
            int flags) __attribute__((const));

        static Tcl_Obj* TraceSet (Tcl_Interp *interp, char *name1, char *name2,
            int flags);

  protected:
        virtual int init();
        virtual int destroy();
        static std::unordered_map<String, link_var_hook_t> link_var_hooks;

  public:
        ScriptInterpTCL() : ScriptInterp(), interp(nullptr), trace_ptrs() {
          init();
        }
        virtual ~ScriptInterpTCL() {
          CmdHandlerData.clear();
          link_var_hooks.clear();
          trace_ptrs.clear();
          destroy();
        }

        virtual String eval(const String& script);
        virtual LoadError loadScript(const String& fileName, String& resultStr);

        template<typename ReturnType, typename... Params>
        inline void createCommand(const String& cmdName,
            ReturnType(*callback)(Params...), const char* usage = nullptr,
            size_t min_params = size_t(-1)) {
          _createCommand(cmdName,
              std::make_unique<ScriptCommandHandlerTCL<ReturnType, Params...>>(
                callback),
              usage, min_params == size_t(-1) ? sizeof...(Params) : min_params,
              sizeof...(Params));
        }

        virtual void deleteCommand(const String& cmdName) {
          CmdHandlerData.erase(cmdName);
        }

        /* Variable linking */

        /**
         * @brief Link a C variable to the interp
         * @param varName The name to create the variable as
         * @param var The variable to link to
         */
        template <typename T>
        inline void linkVar(const String& varName, T& var,
            link_var_hook_t hook_func = nullptr) {
          setupTraces(varName, (ClientData) &var,
              (Tcl_VarTraceProc*) tcl_traceGet<const T>,
              (Tcl_VarTraceProc*) tcl_traceSet<T>, hook_func);
        }

        /**
         * @brief Link a C variable to the interp
         * @param varName The name to create the variable as
         * @param var The variable to link to
         */
        template <typename T>
        inline void linkVar(const String& varName, T* var, size_t size,
            link_var_hook_t hook_func = nullptr) {
          auto data = std::make_unique<trace_ptr_data>(var, size);
          setupTraces(varName, (ClientData) data.get(),
              (Tcl_VarTraceProc*) tcl_traceGetPtrData<const T*>,
              (Tcl_VarTraceProc*) tcl_traceSetPtr<T*>, hook_func);
          trace_ptrs[varName] = std::move(data);
        }

        /**
         * @brief Link a const C variable to the interp
         * @param varName The name to create the variable as
         * @param var The variable to link to
         * @note The variable will be created as read-only
         */
        template <typename T>
        inline void linkVar(const String& varName, const T* var,
            link_var_hook_t hook_func = nullptr) {
          setupTraces(varName, (ClientData) var,
              (Tcl_VarTraceProc*) tcl_traceGetPtr<const T*>,
              (Tcl_VarTraceProc*) TraceSetRO, hook_func);
        }

        /**
         * @brief Link a const C variable to the interp
         * @param varName The name to create the variable as
         * @param var The variable to link to
         * @note The variable will be created as read-only
         */
        template <typename T>
        inline void linkVar(const String& varName, const T& var,
            link_var_hook_t hook_func = nullptr) {
          setupTraces(varName, (ClientData) &var,
              (Tcl_VarTraceProc*) tcl_traceGet<const T>,
              (Tcl_VarTraceProc*) TraceSetRO, hook_func);
        }

        /**
         * @brief Remove a variable from the interp
         * @param varName The name of the variable to remove
         */
        virtual void unlinkVar(const String& varName) {
          Tcl_UnsetVar(interp, *varName, TCL_GLOBAL_ONLY);
          link_var_hooks.erase(varName);
          trace_ptrs.erase(varName);
        }

        virtual script_type type() const __attribute__((const)) {
          return SCRIPT_TYPE_TCL;
        }
};

inline Tcl_Obj* c_to_tcl_cast<const int8_t&>::from(const int8_t& value,
    Tcl_Interp* interp) {
  return c_to_tcl_cast<const int16_t&>::from(value, interp);
}

inline Tcl_Obj* c_to_tcl_cast<const uint8_t&>::from(const uint8_t& value,
    Tcl_Interp* interp) {
  return c_to_tcl_cast<const int8_t&>::from(value, interp);
}

inline Tcl_Obj* c_to_tcl_cast<const int16_t&>::from(const int16_t& value,
    Tcl_Interp* interp) {
  return c_to_tcl_cast<const int32_t&>::from(value, interp);
}

inline Tcl_Obj* c_to_tcl_cast<const uint16_t&>::from(const uint16_t& value,
    Tcl_Interp* interp) {
  return c_to_tcl_cast<const int16_t&>::from(value, interp);
}

inline Tcl_Obj* c_to_tcl_cast<const int32_t&>::from(const int32_t& value,
    Tcl_Interp* interp) {
  return Tcl_NewIntObj(value);
}

inline Tcl_Obj* c_to_tcl_cast<const uint32_t&>::from(const uint32_t& value,
    Tcl_Interp* interp) {
  return c_to_tcl_cast<const int32_t&>::from(value, interp);
}

inline Tcl_Obj* c_to_tcl_cast<const int64_t&>::from(const int64_t& value,
    Tcl_Interp* interp) {
  return Tcl_NewLongObj(value);
}

inline Tcl_Obj* c_to_tcl_cast<const uint64_t&>::from(const uint64_t& value,
    Tcl_Interp* interp) {
  return c_to_tcl_cast<const int64_t&>::from(value, interp);
}

inline Tcl_Obj* c_to_tcl_cast<const double&>::from(const double& value,
    Tcl_Interp* interp) {
  return Tcl_NewDoubleObj(value);
}

inline Tcl_Obj* c_to_tcl_cast<const String&>::from(const String& value,
    Tcl_Interp* interp) {
  return (value.length() < INT_MAX) ? Tcl_NewStringObj(
      value.cbegin(), value.length()) : nullptr;
}

inline Tcl_Obj* c_to_tcl_cast<const char *>::from(const char* value,
    Tcl_Interp* interp) {
  return c_to_tcl_cast<const String&>::from(value, interp);
}

inline Tcl_Obj* c_to_tcl_cast<const bool&>::from(const bool& value,
    Tcl_Interp* interp) {
  return Tcl_NewBooleanObj(value);
}

BDLIB_NS_END
#endif /* USE_SCRIPT_TCL */
#endif /* _BD_SCRIPTINTERPTCL_H */
/* vim: set sts=2 sw=2 ts=8 et: */
