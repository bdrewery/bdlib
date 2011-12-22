#ifndef BD_SCRIPTINTERPTCLCALLBACKS_H
#define BD_SCRIPTINTERPTCLCALLBACKS_H

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

/**
 * @class VariadicHelperTCL
 * @brief Lookup the index of the Tcl_Obj to unpack in ScriptCallbackTCL::call
 * @todo this may violate sequence point rules (-Wsequence-point), a recursive template should be used instead
 */
class VariadicHelperTCL {
  private:
    size_t _index;
    size_t _objc;
    Tcl_Obj* CONST *_objv;
    VariadicHelperTCL() = delete;
  public:
    VariadicHelperTCL(size_t objc, Tcl_Obj* CONST *objv) : _index(objc - 1), _objc(objc), _objv(objv) {}
    inline Tcl_Obj* next_value() {
      return _objv[_index--];
    }
};

template <typename ReturnType, typename... Params>
class ScriptCallbackTCL : public ScriptCallbackTCLBase {
  typedef ReturnType (*function_t)(Params...);

  private:
    function_t _callback;

  public:
    ScriptCallbackTCL(function_t callback) : _callback(callback) {};
    virtual void call(int objc, void* const argv[], void* proxy_data) {
      Tcl_Obj* CONST *objv = reinterpret_cast<Tcl_Obj* CONST *>(argv);
      Tcl_Interp* interp = static_cast<Tcl_Interp*>(proxy_data);
      VariadicHelperTCL variadic_helper(objc, objv);
      ScriptCallbackDispatchTCL<ReturnType, Params...>::dispatch(interp, _callback,
          std::move(tcl_to_c_cast<Params>::from(std::move(variadic_helper.next_value())))...
      );
    }
};
#endif
