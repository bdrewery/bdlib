#ifndef BD_SCRIPTINTERPTCLCALLBACKS_H
#define BD_SCRIPTINTERPTCLCALLBACKS_H

#include "indices.h"

/**
 * @brief Execute the callback and set the return type
 */
template <typename ReturnType, typename... Params>
struct ScriptCallbackDispatchTCL {
  typedef ReturnType (*function_t)(Params...);
  static inline void dispatch(Tcl_Interp* interp, function_t callback,
      Params&&... args) {
    const ReturnType result(std::move(callback(std::forward<Params>(args)...)));
    Tcl_SetObjResult(interp,
        std::move(c_to_tcl_cast<const ReturnType&>::from(std::move(result),
            interp)));
  }
};

// Specialize for void return-type
template <typename... Params>
struct ScriptCallbackDispatchTCL<void, Params...> {
  typedef void (*function_t)(Params...);
  static inline void dispatch(Tcl_Interp* interp, function_t callback,
      Params&&... args) {
    callback(std::forward<Params>(args)...);
  }
};

template <typename ReturnType, typename... Params>
class ScriptCommandHandlerTCL : public ScriptCommandHandlerTCLBase {
  typedef ReturnType (*function_t)(Params...);

  private:
    function_t _callback;

    template<std::size_t... Indices>
    inline void real_call(__attribute__((__unused__)) size_t argc,
        void* const argv[], __attribute__((__unused__)) ScriptInterp* si,
        void *proxy_data, indices<Indices...>) {
      auto objv = reinterpret_cast<Tcl_Obj* CONST *>(argv);
      auto interp = static_cast<Tcl_Interp*>(proxy_data);
      // Doing argc check to pass default params in if not enough were passed
      // to the handler.
      ScriptCallbackDispatchTCL<ReturnType, Params...>::dispatch(interp,
          _callback,
          (
           ((argc - 1) >= Indices + 1) ?
           (std::move(tcl_to_c_cast<Params>::from(std::move(objv[Indices + 1]),
                                                  si))) :
           (std::move(Params()))
          )...
      );
    }

  public:
    ScriptCommandHandlerTCL(function_t callback) : _callback(callback) {};
    inline virtual void call(size_t argc, void* const argv[], ScriptInterp* si,
        void *proxy_data) {
      real_call(argc, argv, si, proxy_data, make_indices<sizeof...(Params)>());
    }
};
#endif
/* vim: set sts=2 sw=2 ts=8 et: */
