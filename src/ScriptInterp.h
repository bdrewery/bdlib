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
    virtual void call(size_t argc, void* const argv[], void *proxy_data = NULL) = 0;
};

/**
 * @class ScriptInterp
 * @brief Handles generalized script interpreter access
 */
class ScriptInterp {
  private:
        ScriptInterp(const ScriptInterp&) = delete;
        ScriptInterp& operator=(const ScriptInterp&) = delete;

  protected:
        virtual int init() = 0;
        virtual int destroy() = 0;

  public:
        struct script_cmd_handler_clientdata {
          ScriptInterp* si;
          ScriptCallbackBase* callback_proxy;
          size_t callbackParamCount;
          script_cmd_handler_clientdata (ScriptInterp* _si, ScriptCallbackBase* _callback_proxy, size_t _callbackParamCount) :
            si(_si),
            callback_proxy(_callback_proxy),
            callbackParamCount(_callbackParamCount) {};
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

        template<typename InterpType, typename ReturnType, typename... Params>
        static void createCommand(InterpType& si, const String& cmdName, ReturnType(*callback)(Params...)) {
          si.createCommand(cmdName, callback);
        }

        /**
         * @brief Remove a command from the interp
         * @param name The name of the command to remove
         */
        virtual void deleteCommand(const String& name) = 0;

        /**
         * @brief Link a C variable to the interp
         * @param si The ScriptInterp to link the variable to
         * @param varName The name to create the variable as
         * @param var The variable to link to
         * @note This is dispatching to the derived class due to being unable to do specialized template methods
         */
        template <typename Interp, typename T>
          static void linkVar(Interp& si, const String& varName, T& var) {
            si.linkVar(varName, var);
          };

        /**
         * @brief Remove a variable from the interp
         * @param varName The name of the variable to remove
         */
        virtual void unlinkVar(const String& varName) = 0;
};


BDLIB_NS_END
#endif /* _BD_SCRIPTINTERP_H */
