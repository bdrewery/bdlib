#ifndef BD_SCRIPTINTERPTCLCALLBACKS_H
#define BD_SCRIPTINTERPTCLCALLBACKS_H
// Patterns taken from cpptcl

/**
 * @class ScriptCallbackDispatchTCL
 * @brief Execute the callback and set the return type
 */
template <typename ReturnType>
struct ScriptCallbackDispatchTCL {
  template<typename Func>
    static void dispatch(Tcl_Interp* interp, Func callback) {
      ReturnType result = callback();
      Tcl_SetObjResult(interp, c_to_tcl_cast<ReturnType>::from(result));
    }
  template<typename Func, typename T1>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1) {
      ReturnType result = callback(p1);
      Tcl_SetObjResult(interp, c_to_tcl_cast<ReturnType>::from(result));
    }
  template<typename Func, typename T1, typename T2>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1, T2 p2) {
      ReturnType result = callback(p1, p2);
      Tcl_SetObjResult(interp, c_to_tcl_cast<ReturnType>::from(result));
    }
  template<typename Func, typename T1, typename T2, typename T3>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1, T2 p2, T3 p3) {
      ReturnType result = callback(p1, p2, p3);
      Tcl_SetObjResult(interp, c_to_tcl_cast<ReturnType>::from(result));
    }
  template<typename Func, typename T1, typename T2, typename T3, typename T4>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1, T2 p2, T3 p3, T4 p4) {
      ReturnType result = callback(p1, p2, p3, p4);
      Tcl_SetObjResult(interp, c_to_tcl_cast<ReturnType>::from(result));
    }
  template<typename Func, typename T1, typename T2, typename T3, typename T4, typename T5>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5) {
      ReturnType result = callback(p1, p2, p3, p4, p5);
      Tcl_SetObjResult(interp, c_to_tcl_cast<ReturnType>::from(result));
    }
  template<typename Func, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6) {
      ReturnType result = callback(p1, p2, p3, p4, p5, p6);
      Tcl_SetObjResult(interp, c_to_tcl_cast<ReturnType>::from(result));
    }
  template<typename Func, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7) {
      ReturnType result = callback(p1, p2, p3, p4, p5, p6, p7);
      Tcl_SetObjResult(interp, c_to_tcl_cast<ReturnType>::from(result));
    }
  template<typename Func, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8) {
      ReturnType result = callback(p1, p2, p3, p4, p5, p6, p7, p8);
      Tcl_SetObjResult(interp, c_to_tcl_cast<ReturnType>::from(result));
    }
  template<typename Func, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9) {
      ReturnType result = callback(p1, p2, p3, p4, p5, p6, p7, p8, p9);
      Tcl_SetObjResult(interp, c_to_tcl_cast<ReturnType>::from(result));
    }
  template<typename Func, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10) {
      ReturnType result = callback(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
      Tcl_SetObjResult(interp, c_to_tcl_cast<ReturnType>::from(result));
    }
};

// Specialize for void return-type
template <>
struct ScriptCallbackDispatchTCL<void> {
  template<typename Func>
    static void dispatch(Tcl_Interp* interp, Func callback) {
      callback();
    }
  template<typename Func, typename T1>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1) {
      callback(p1);
    }
  template<typename Func, typename T1, typename T2>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1, T2 p2) {
      callback(p1, p2);
    }
  template<typename Func, typename T1, typename T2, typename T3>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1, T2 p2, T3 p3) {
      callback(p1, p2, p3);
    }
  template<typename Func, typename T1, typename T2, typename T3, typename T4>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1, T2 p2, T3 p3, T4 p4) {
      callback(p1, p2, p3, p4);
    }
  template<typename Func, typename T1, typename T2, typename T3, typename T4, typename T5>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5) {
      callback(p1, p2, p3, p4, p5);
    }
  template<typename Func, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6) {
      callback(p1, p2, p3, p4, p5, p6);
    }
  template<typename Func, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7) {
      callback(p1, p2, p3, p4, p5, p6, p7);
    }
  template<typename Func, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8) {
      callback(p1, p2, p3, p4, p5, p6, p7, p8);
    }
  template<typename Func, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9) {
      callback(p1, p2, p3, p4, p5, p6, p7, p8, p9);
    }
  template<typename Func, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
    static void dispatch(Tcl_Interp* interp, Func callback, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10) {
      callback(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
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
          // Doing objc check to pass default params in if not enough were passed to the handler
          objc >= 2 ? tcl_to_c_cast<T1>::from(objv[1]) : T1()
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
          // Doing objc check to pass default params in if not enough were passed to the handler
          objc >= 2 ? tcl_to_c_cast<T1>::from(objv[1]) : T1(),
          objc >= 3 ? tcl_to_c_cast<T2>::from(objv[2]) : T2()
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
          // Doing objc check to pass default params in if not enough were passed to the handler
          objc >= 2 ? tcl_to_c_cast<T1>::from(objv[1]) : T1(),
          objc >= 3 ? tcl_to_c_cast<T2>::from(objv[2]) : T2(),
          objc >= 4 ? tcl_to_c_cast<T3>::from(objv[3]) : T3()
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
          // Doing objc check to pass default params in if not enough were passed to the handler
          objc >= 2 ? tcl_to_c_cast<T1>::from(objv[1]) : T1(),
          objc >= 3 ? tcl_to_c_cast<T2>::from(objv[2]) : T2(),
          objc >= 4 ? tcl_to_c_cast<T3>::from(objv[3]) : T3(),
          objc >= 5 ? tcl_to_c_cast<T4>::from(objv[4]) : T4()
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
          // Doing objc check to pass default params in if not enough were passed to the handler
          objc >= 2 ? tcl_to_c_cast<T1>::from(objv[1]) : T1(),
          objc >= 3 ? tcl_to_c_cast<T2>::from(objv[2]) : T2(),
          objc >= 4 ? tcl_to_c_cast<T3>::from(objv[3]) : T3(),
          objc >= 5 ? tcl_to_c_cast<T4>::from(objv[4]) : T4(),
          objc >= 6 ? tcl_to_c_cast<T5>::from(objv[5]) : T5()
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
          // Doing objc check to pass default params in if not enough were passed to the handler
          objc >= 2 ? tcl_to_c_cast<T1>::from(objv[1]) : T1(),
          objc >= 3 ? tcl_to_c_cast<T2>::from(objv[2]) : T2(),
          objc >= 4 ? tcl_to_c_cast<T3>::from(objv[3]) : T3(),
          objc >= 5 ? tcl_to_c_cast<T4>::from(objv[4]) : T4(),
          objc >= 6 ? tcl_to_c_cast<T5>::from(objv[5]) : T5(),
          objc >= 7 ? tcl_to_c_cast<T6>::from(objv[6]) : T6()
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
          // Doing objc check to pass default params in if not enough were passed to the handler
          objc >= 2 ? tcl_to_c_cast<T1>::from(objv[1]) : T1(),
          objc >= 3 ? tcl_to_c_cast<T2>::from(objv[2]) : T2(),
          objc >= 4 ? tcl_to_c_cast<T3>::from(objv[3]) : T3(),
          objc >= 5 ? tcl_to_c_cast<T4>::from(objv[4]) : T4(),
          objc >= 6 ? tcl_to_c_cast<T5>::from(objv[5]) : T5(),
          objc >= 7 ? tcl_to_c_cast<T6>::from(objv[6]) : T6(),
          objc >= 8 ? tcl_to_c_cast<T7>::from(objv[7]) : T7()
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
          // Doing objc check to pass default params in if not enough were passed to the handler
          objc >= 2 ? tcl_to_c_cast<T1>::from(objv[1]) : T1(),
          objc >= 3 ? tcl_to_c_cast<T2>::from(objv[2]) : T2(),
          objc >= 4 ? tcl_to_c_cast<T3>::from(objv[3]) : T3(),
          objc >= 5 ? tcl_to_c_cast<T4>::from(objv[4]) : T4(),
          objc >= 6 ? tcl_to_c_cast<T5>::from(objv[5]) : T5(),
          objc >= 7 ? tcl_to_c_cast<T6>::from(objv[6]) : T6(),
          objc >= 8 ? tcl_to_c_cast<T7>::from(objv[7]) : T7(),
          objc >= 9 ? tcl_to_c_cast<T8>::from(objv[8]) : T8()
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
          // Doing objc check to pass default params in if not enough were passed to the handler
          objc >= 2 ? tcl_to_c_cast<T1>::from(objv[1]) : T1(),
          objc >= 3 ? tcl_to_c_cast<T2>::from(objv[2]) : T2(),
          objc >= 4 ? tcl_to_c_cast<T3>::from(objv[3]) : T3(),
          objc >= 5 ? tcl_to_c_cast<T4>::from(objv[4]) : T4(),
          objc >= 6 ? tcl_to_c_cast<T5>::from(objv[5]) : T5(),
          objc >= 7 ? tcl_to_c_cast<T6>::from(objv[6]) : T6(),
          objc >= 8 ? tcl_to_c_cast<T7>::from(objv[7]) : T7(),
          objc >= 9 ? tcl_to_c_cast<T8>::from(objv[8]) : T8(),
          objc >= 10 ? tcl_to_c_cast<T9>::from(objv[9]) : T9()
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
          // Doing objc check to pass default params in if not enough were passed to the handler
          objc >= 2 ? tcl_to_c_cast<T1>::from(objv[1]) : T1(),
          objc >= 3 ? tcl_to_c_cast<T2>::from(objv[2]) : T2(),
          objc >= 4 ? tcl_to_c_cast<T3>::from(objv[3]) : T3(),
          objc >= 5 ? tcl_to_c_cast<T4>::from(objv[4]) : T4(),
          objc >= 6 ? tcl_to_c_cast<T5>::from(objv[5]) : T5(),
          objc >= 7 ? tcl_to_c_cast<T6>::from(objv[6]) : T6(),
          objc >= 8 ? tcl_to_c_cast<T7>::from(objv[7]) : T7(),
          objc >= 9 ? tcl_to_c_cast<T8>::from(objv[8]) : T8(),
          objc >= 10 ? tcl_to_c_cast<T9>::from(objv[9]) : T9(),
          objc >= 11 ? tcl_to_c_cast<T10>::from(objv[10]) : T10()
      );
    }
};
#endif
