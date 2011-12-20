#ifndef BD_SCRIPTINTERPTCLCALLBACKS_H
#define BD_SCRIPTINTERPTCLCALLBACKS_H
// Patterns taken from cpptcl

/**
 * @class ScriptCallbackDispatchTCL
 * @brief Execute the callback and set the return type
 */
template <typename ReturnType>
struct ScriptCallbackDispatchTCL {
  template<typename Func, typename... Params>
    static void dispatch(Tcl_Interp* interp, Func callback, Params... args) {
      ReturnType result = callback(args...);
      Tcl_SetObjResult(interp, c_to_tcl_cast<ReturnType>::from(result));
    }
};

// Specialize for void return-type
template <>
struct ScriptCallbackDispatchTCL<void> {
  template<typename Func, typename... Params>
    static void dispatch(Tcl_Interp* interp, Func callback, Params... args) {
      callback(args...);
    }
};


template <typename ReturnType>
class ScriptCallbackTCL0 : public ScriptCallbackTCLBase {
  typedef ReturnType (*function_t)();

  private:
    function_t _callback;

  public:
    ScriptCallbackTCL0(function_t callback) : _callback(callback) {};
    virtual void call(int objc, void* const argv[], void* proxy_data) {
      Tcl_Interp* interp = static_cast<Tcl_Interp*>(proxy_data);
      ScriptCallbackDispatchTCL<ReturnType>::dispatch(interp, _callback);
    }
};

template <typename ReturnType, typename T1>
class ScriptCallbackTCL1 : public ScriptCallbackTCLBase {
  typedef ReturnType (*function_t)(T1);

  private:
    function_t _callback;

  public:
    ScriptCallbackTCL1(function_t callback) : _callback(callback) {};
    virtual void call(int objc, void* const argv[], void* proxy_data) {
      Tcl_Obj* CONST *objv = reinterpret_cast<Tcl_Obj* CONST *>(argv);
      Tcl_Interp* interp = static_cast<Tcl_Interp*>(proxy_data);
      ScriptCallbackDispatchTCL<ReturnType>::dispatch(interp, _callback,
          tcl_to_c_cast<T1>::from(objv[1])
      );
    }
};

template <typename ReturnType, typename T1, typename T2>
class ScriptCallbackTCL2 : public ScriptCallbackTCLBase {
  typedef ReturnType (*function_t)(T1, T2);

  private:
    function_t _callback;

  public:
    ScriptCallbackTCL2(function_t callback) : _callback(callback) {};
    virtual void call(int objc, void* const argv[], void* proxy_data) {
      Tcl_Obj* CONST *objv = reinterpret_cast<Tcl_Obj* CONST *>(argv);
      Tcl_Interp* interp = static_cast<Tcl_Interp*>(proxy_data);
      ScriptCallbackDispatchTCL<ReturnType>::dispatch(interp, _callback,
          tcl_to_c_cast<T1>::from(objv[1]),
          tcl_to_c_cast<T2>::from(objv[2])
      );
    }
};

template <typename ReturnType, typename T1, typename T2, typename T3>
class ScriptCallbackTCL3 : public ScriptCallbackTCLBase {
  typedef ReturnType (*function_t)(T1, T2, T3);

  private:
    function_t _callback;

  public:
    ScriptCallbackTCL3(function_t callback) : _callback(callback) {};
    virtual void call(int objc, void* const argv[], void* proxy_data) {
      Tcl_Obj* CONST *objv = reinterpret_cast<Tcl_Obj* CONST *>(argv);
      Tcl_Interp* interp = static_cast<Tcl_Interp*>(proxy_data);
      ScriptCallbackDispatchTCL<ReturnType>::dispatch(interp, _callback,
          tcl_to_c_cast<T1>::from(objv[1]),
          tcl_to_c_cast<T2>::from(objv[2]),
          tcl_to_c_cast<T3>::from(objv[3])
      );
    }
};

template <typename ReturnType, typename T1, typename T2, typename T3, typename T4>
class ScriptCallbackTCL4 : public ScriptCallbackTCLBase {
  typedef ReturnType (*function_t)(T1, T2, T3, T4);

  private:
    function_t _callback;

  public:
    ScriptCallbackTCL4(function_t callback) : _callback(callback) {};
    virtual void call(int objc, void* const argv[], void* proxy_data) {
      Tcl_Obj* CONST *objv = reinterpret_cast<Tcl_Obj* CONST *>(argv);
      Tcl_Interp* interp = static_cast<Tcl_Interp*>(proxy_data);
      ScriptCallbackDispatchTCL<ReturnType>::dispatch(interp, _callback,
          tcl_to_c_cast<T1>::from(objv[1]),
          tcl_to_c_cast<T2>::from(objv[2]),
          tcl_to_c_cast<T3>::from(objv[3]),
          tcl_to_c_cast<T4>::from(objv[4])
      );
    }
};

template <typename ReturnType, typename T1, typename T2, typename T3, typename T4, typename T5>
class ScriptCallbackTCL5 : public ScriptCallbackTCLBase {
  typedef ReturnType (*function_t)(T1, T2, T3, T4, T5);

  private:
    function_t _callback;

  public:
    ScriptCallbackTCL5(function_t callback) : _callback(callback) {};
    virtual void call(int objc, void* const argv[], void* proxy_data) {
      Tcl_Obj* CONST *objv = reinterpret_cast<Tcl_Obj* CONST *>(argv);
      Tcl_Interp* interp = static_cast<Tcl_Interp*>(proxy_data);
      ScriptCallbackDispatchTCL<ReturnType>::dispatch(interp, _callback,
          tcl_to_c_cast<T1>::from(objv[1]),
          tcl_to_c_cast<T2>::from(objv[2]),
          tcl_to_c_cast<T3>::from(objv[3]),
          tcl_to_c_cast<T4>::from(objv[4]),
          tcl_to_c_cast<T5>::from(objv[5])
      );
    }
};

template <typename ReturnType, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class ScriptCallbackTCL6 : public ScriptCallbackTCLBase {
  typedef ReturnType (*function_t)(T1, T2, T3, T4, T5, T6);

  private:
    function_t _callback;

  public:
    ScriptCallbackTCL6(function_t callback) : _callback(callback) {};
    virtual void call(int objc, void* const argv[], void* proxy_data) {
      Tcl_Obj* CONST *objv = reinterpret_cast<Tcl_Obj* CONST *>(argv);
      Tcl_Interp* interp = static_cast<Tcl_Interp*>(proxy_data);
      ScriptCallbackDispatchTCL<ReturnType>::dispatch(interp, _callback,
          tcl_to_c_cast<T1>::from(objv[1]),
          tcl_to_c_cast<T2>::from(objv[2]),
          tcl_to_c_cast<T3>::from(objv[3]),
          tcl_to_c_cast<T4>::from(objv[4]),
          tcl_to_c_cast<T5>::from(objv[5]),
          tcl_to_c_cast<T6>::from(objv[6])
      );
    }
};

template <typename ReturnType, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class ScriptCallbackTCL7 : public ScriptCallbackTCLBase {
  typedef ReturnType (*function_t)(T1, T2, T3, T4, T5, T6, T7);

  private:
    function_t _callback;

  public:
    ScriptCallbackTCL7(function_t callback) : _callback(callback) {};
    virtual void call(int objc, void* const argv[], void* proxy_data) {
      Tcl_Obj* CONST *objv = reinterpret_cast<Tcl_Obj* CONST *>(argv);
      Tcl_Interp* interp = static_cast<Tcl_Interp*>(proxy_data);
      ScriptCallbackDispatchTCL<ReturnType>::dispatch(interp, _callback,
          tcl_to_c_cast<T1>::from(objv[1]),
          tcl_to_c_cast<T2>::from(objv[2]),
          tcl_to_c_cast<T3>::from(objv[3]),
          tcl_to_c_cast<T4>::from(objv[4]),
          tcl_to_c_cast<T5>::from(objv[5]),
          tcl_to_c_cast<T6>::from(objv[6]),
          tcl_to_c_cast<T7>::from(objv[7])
      );
    }
};

template <typename ReturnType, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class ScriptCallbackTCL8 : public ScriptCallbackTCLBase {
  typedef ReturnType (*function_t)(T1, T2, T3, T4, T5, T6, T7, T8);

  private:
    function_t _callback;

  public:
    ScriptCallbackTCL8(function_t callback) : _callback(callback) {};
    virtual void call(int objc, void* const argv[], void* proxy_data) {
      Tcl_Obj* CONST *objv = reinterpret_cast<Tcl_Obj* CONST *>(argv);
      Tcl_Interp* interp = static_cast<Tcl_Interp*>(proxy_data);
      ScriptCallbackDispatchTCL<ReturnType>::dispatch(interp, _callback,
          tcl_to_c_cast<T1>::from(objv[1]),
          tcl_to_c_cast<T2>::from(objv[2]),
          tcl_to_c_cast<T3>::from(objv[3]),
          tcl_to_c_cast<T4>::from(objv[4]),
          tcl_to_c_cast<T5>::from(objv[5]),
          tcl_to_c_cast<T6>::from(objv[6]),
          tcl_to_c_cast<T7>::from(objv[7]),
          tcl_to_c_cast<T8>::from(objv[8])
      );
    }
};

template <typename ReturnType, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class ScriptCallbackTCL9 : public ScriptCallbackTCLBase {
  typedef ReturnType (*function_t)(T1, T2, T3, T4, T5, T6, T7, T8, T9);

  private:
    function_t _callback;

  public:
    ScriptCallbackTCL9(function_t callback) : _callback(callback) {};
    virtual void call(int objc, void* const argv[], void* proxy_data) {
      Tcl_Obj* CONST *objv = reinterpret_cast<Tcl_Obj* CONST *>(argv);
      Tcl_Interp* interp = static_cast<Tcl_Interp*>(proxy_data);
      ScriptCallbackDispatchTCL<ReturnType>::dispatch(interp, _callback,
          tcl_to_c_cast<T1>::from(objv[1]),
          tcl_to_c_cast<T2>::from(objv[2]),
          tcl_to_c_cast<T3>::from(objv[3]),
          tcl_to_c_cast<T4>::from(objv[4]),
          tcl_to_c_cast<T5>::from(objv[5]),
          tcl_to_c_cast<T6>::from(objv[6]),
          tcl_to_c_cast<T7>::from(objv[7]),
          tcl_to_c_cast<T8>::from(objv[8]),
          tcl_to_c_cast<T9>::from(objv[9])
      );
    }
};

template <typename ReturnType, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class ScriptCallbackTCL10 : public ScriptCallbackTCLBase {
  typedef ReturnType (*function_t)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);

  private:
    function_t _callback;

  public:
    ScriptCallbackTCL10(function_t callback) : _callback(callback) {};
    virtual void call(int objc, void* const argv[], void* proxy_data) {
      Tcl_Obj* CONST *objv = reinterpret_cast<Tcl_Obj* CONST *>(argv);
      Tcl_Interp* interp = static_cast<Tcl_Interp*>(proxy_data);
      ScriptCallbackDispatchTCL<ReturnType>::dispatch(interp, _callback,
          tcl_to_c_cast<T1>::from(objv[1]),
          tcl_to_c_cast<T2>::from(objv[2]),
          tcl_to_c_cast<T3>::from(objv[3]),
          tcl_to_c_cast<T4>::from(objv[4]),
          tcl_to_c_cast<T5>::from(objv[5]),
          tcl_to_c_cast<T6>::from(objv[6]),
          tcl_to_c_cast<T7>::from(objv[7]),
          tcl_to_c_cast<T8>::from(objv[8]),
          tcl_to_c_cast<T9>::from(objv[9]),
          tcl_to_c_cast<T10>::from(objv[10])
      );
    }
};
#endif
