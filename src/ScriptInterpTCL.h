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

#include <sys/types.h>

#include <tcl.h>

BDLIB_NS_BEGIN

class ScriptInterpTCL : public ScriptInterp {
  private:
    Tcl_Interp *interp;
  protected:

  public:
        ScriptInterpTCL() : ScriptInterp(), interp(NULL)  {};
        ScriptInterpTCL(const ScriptInterpTCL& ginterp) : ScriptInterp(ginterp), interp(NULL)  {};
        virtual ~ScriptInterpTCL() {};

        inline ScriptInterpTCL& operator=(const ScriptInterpTCL& si) {
          if (&si == this) return (*this);
          destroy();
          ScriptInterp::operator= (si);
          interp = si.interp;
          return (*this);
        }

        virtual int init();
        virtual int destroy();
        virtual String eval(const String& script);
        virtual void linkVar(const String& name, String& var);
        static const char* TraceGetString (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags);
        static const char* TraceSetString (ClientData clientData, Tcl_Interp *interp, char *name1, char *name2, int flags);
};


BDLIB_NS_END
#endif /* _BD_SCRIPTINTERPTCL_H */
