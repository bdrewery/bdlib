#ifndef BD_SCRIPTINTERPTCLCALLBACKS_H
#define BD_SCRIPTINTERPTCLCALLBACKS_H

#include "indices.h"

/**
 * @class ScriptCallbackDispatchTCL
 * @brief Execute the callback and set the return type
 */
template <typename ReturnType, typename... Params>
struct ScriptCallbackDispatchTCL {
  typedef ReturnType (*function_t)(Params...);
  static inline void dispatch(Tcl_Interp* interp, function_t callback, Params&&... args) {
    ReturnType result = std::move(callback(std::forward<Params>(args)...));
    Tcl_SetObjResult(interp, std::move(c_to_tcl_cast<ReturnType>::from(std::move(result))));
  }
};

// Specialize for void return-type
template <typename... Params>
struct ScriptCallbackDispatchTCL<void, Params...> {
  typedef void (*function_t)(Params...);
  static inline void dispatch(Tcl_Interp* interp, function_t callback, Params&&... args) {
    callback(std::forward<Params>(args)...);
  }
};

template <typename ReturnType, typename... Params>
class ScriptCallbackTCL : public ScriptCallbackTCLBase {
  typedef ReturnType (*function_t)(Params...);

  private:
    function_t _callback;

    template<std::size_t... Indices>
    inline void real_call(size_t argc, void* const argv[], void* proxy_data, indices<Indices...>) {
      Tcl_Obj* CONST *objv = reinterpret_cast<Tcl_Obj* CONST *>(argv);
      Tcl_Interp* interp = static_cast<Tcl_Interp*>(proxy_data);
      ScriptCallbackDispatchTCL<ReturnType, Params...>::dispatch(interp, _callback,
          std::move(tcl_to_c_cast<Params>::from(std::move(objv[Indices + 1])))...
      );
    }

  public:
    ScriptCallbackTCL(function_t callback) : _callback(callback) {};
    inline virtual void call(size_t argc, void* const argv[], void* proxy_data) {
      real_call(argc, argv, proxy_data, make_indices<sizeof...(Params)>());
    }
};
#endif
