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

class ScriptArgsTCL : public ScriptArgs {
  private:
    Tcl_Obj** my_objv;

    // Don't allow copying
    ScriptArgsTCL(const ScriptArgsTCL&) : ScriptArgs(), my_objv() {};
    ScriptArgsTCL& operator=(const ScriptArgsTCL&) {return *this;};
  public:
    ScriptArgsTCL() : ScriptArgs(), my_objv() {};
    ScriptArgsTCL(int objc, Tcl_Obj* CONST objv[]) : ScriptArgs(objc), my_objv() {
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
      //FIXME: DRY with TraceSetString
      int len = 0;
      char *cstr = Tcl_GetStringFromObj(my_objv[index], &len);
      if (!cstr) return String();
      return String(cstr, len);
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
        static void tcl_command_ondelete(ClientData clientData);
  protected:
        virtual int init();
        virtual int destroy();

  public:
        ScriptInterpTCL() : ScriptInterp(), interp(NULL) {init();};
        virtual ~ScriptInterpTCL() {destroy();};

        virtual String eval(const String& script);

        virtual void createCommand(const String& name, script_callback_string_t callback, script_clientdata_t clientData = NULL);
        virtual void createCommand(const String& name, script_callback_int_t callback, script_clientdata_t clientData = NULL);

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
