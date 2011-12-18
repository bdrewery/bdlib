/* ScriptInterp.h
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
#ifndef _BD_SCRIPTINTERP_H
#define _BD_SCRIPTINTERP_H 1

#include "bdlib.h"
#include "String.h"
#include "Array.h"

#include <sys/types.h>

BDLIB_NS_BEGIN

#define SCRIPT_BADARGS(nl, nh, example) do {                              \
	if ((args.length() < (nl)) || (args.length() > (nh))) {           \
                return_data.type = bd::SCRIPT_RETURN_TYPE_STRING;         \
                return_data.value_string = "wrong # args: should be \"" + \
                               args.getArgString(0) + (example) + "\"";   \
                return bd::SCRIPT_ERROR;                                  \
	}                                                                 \
} while (0)

class ScriptCallbackBase {
  public:
    virtual ~ScriptCallbackBase() {};
    virtual void call(int argc, void* const argv[], void *proxy_data = NULL) = 0;
};

/**
 * @class ScriptInterp
 * @brief Handles generalized script interpreter access
 */
class ScriptInterp {
  private:
        // Don't allow copying
        ScriptInterp(const ScriptInterp&)  {};
        ScriptInterp& operator=(const ScriptInterp&) {return *this;};

  protected:
        virtual int init() = 0;
        virtual int destroy() = 0;

  public:
        typedef void* script_clientdata_t;

        struct script_cmd_handler_clientdata {
          ScriptInterp* si;
          script_clientdata_t clientData;
          ScriptCallbackBase* callback_proxy;
          script_cmd_handler_clientdata (ScriptInterp* _si, script_clientdata_t _clientData, ScriptCallbackBase* _callback_proxy) : si(_si), clientData(_clientData), callback_proxy(_callback_proxy) {};
        };

        ScriptInterp() {};
        virtual ~ScriptInterp() {};

        /**
         * @param script The script to evaluate
         */
        virtual String eval(const String& script) = 0;

        enum LoadError {
          SCRIPT_LOAD_OK,
          SCRIPT_LOAD_ERROR,
          SCRIPT_LOAD_WRONG_INTERP
        };
        /**
         * @brief Load script file
         * @param fileName The file to load
         * @param resultStr String to hold error output from interp
         */
        virtual LoadError loadScript(const String& fileName, String& resultStr) = 0;

        /**
         * @brief Remove a command from the interp
         * @param name The name of the command to remove
         */
        virtual void deleteCommand(const String& name) = 0;

        /**
         * @brief Remove a variable from the interp
         * @param name The name of the variable to remove
         */
        virtual void unlinkVar(const String& name) {};

        /**
         * @brief Return the class name as a String
         * This is only needed for calling specialized template methods from the derived class. See Unit Tests for examples.
         * @sa http://stackoverflow.com/questions/2914081/c-member-function-template-in-derived-class-how-to-call-it-from-base-class
         */
        virtual String type() const = 0;
};


BDLIB_NS_END
#endif /* _BD_SCRIPTINTERP_H */
