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
#include <memory>

BDLIB_NS_BEGIN

class ScriptInterp;

class ScriptCommandHandlerBase {
  public:
    virtual ~ScriptCommandHandlerBase() {};
    virtual void call(size_t argc, void* const argv[], ScriptInterp* si,
        void *proxy_data = NULL) = 0;
};

class ScriptCallbacker {
  private:
    ScriptCallbacker(const ScriptCallbacker&) = delete;
    ScriptCallbacker& operator=(const ScriptCallbacker&) = delete;
  public:
    ScriptInterp* si;
    const String cmd;

    ScriptCallbacker(ScriptInterp* _si, const String& _cmd) :
      si(_si), cmd(_cmd) {};
    virtual ~ScriptCallbacker() {};
    virtual String call(const Array<String>& params = Array<String>()) = 0;
};
typedef std::shared_ptr<ScriptCallbacker> ScriptCallbackerPtr;

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

        struct ScriptCmd {
          ScriptCmd(const ScriptCmd&) = delete;
          ScriptCmd& operator=(const ScriptCmd&) = delete;
          ScriptInterp* si;
          const String cmdName;
          std::unique_ptr<ScriptCommandHandlerBase> callback_proxy;
          const char* usage;
          size_t callbackParamMin;
          size_t callbackParamMax;
          bool registered;
          ScriptCmd(ScriptInterp* _si, const String &_cmdName,
              std::unique_ptr<ScriptCommandHandlerBase> _callback_proxy,
              const char* _usage, size_t _callbackParamMin,
              size_t _callbackParamMax) :
            si(_si),
            cmdName(_cmdName),
            callback_proxy(std::move(_callback_proxy)),
            usage(_usage),
            callbackParamMin(_callbackParamMin),
            callbackParamMax(_callbackParamMax),
            registered(false)
          {}
          virtual ~ScriptCmd() {};
          virtual void registerCmd() {
            registered = true;
          }
          virtual void unregisterCmd() {
            registered = false;
          }
        };
        typedef std::shared_ptr<ScriptCmd> ScriptCmdPtr;
  public:
        enum script_type {
          SCRIPT_TYPE_TCL,
        };
        typedef void (*link_var_hook_t)(const void*, const void*);

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
        virtual LoadError loadScript(const String& fileName,
            String& resultStr) = 0;

        template<typename InterpType, typename ReturnType, typename... Params>
        static void createCommand(InterpType& si,
            const String& cmdName,
            ReturnType(*callback)(Params...), const char* usage = nullptr,
            size_t min_params = size_t(-1)) {
          si.createCommand(cmdName, callback, usage, min_params);
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
        }

        /**
         * @brief Remove a variable from the interp
         * @param varName The name of the variable to remove
         */
        virtual void unlinkVar(const String& varName) = 0;

        // Used for downcasting hack due to no virtual templates
        virtual script_type type() const __attribute__((pure)) = 0;
};


BDLIB_NS_END
#endif /* _BD_SCRIPTINTERP_H */
/* vim: set sts=2 sw=2 ts=8 et: */
