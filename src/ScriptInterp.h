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

#include <sys/types.h>

BDLIB_NS_BEGIN

class ScriptInterp {
  private:
        // Don't allow copying
        ScriptInterp(const ScriptInterp&)  {};
        ScriptInterp& operator=(const ScriptInterp&) {return *this;};

  protected:
        virtual int init() = 0;
        virtual int destroy() = 0;
        typedef intptr_t (*script_callback_t) ();

  public:
        typedef void* script_clientdata_t;
        typedef String (*script_callback_string_t)(ScriptInterp* Interp, script_clientdata_t clientData);
        typedef int (*script_callback_int_t)(ScriptInterp* Interp, script_clientdata_t clientData);
        struct script_callback_clientdata_t {
          ScriptInterp* si;
          script_clientdata_t clientData;
          script_callback_t callback;
          script_callback_clientdata_t (ScriptInterp* _si, script_clientdata_t _clientData, script_callback_t _callback) : si(_si), clientData(_clientData), callback(_callback) {};
        };

        ScriptInterp() {};
        virtual ~ScriptInterp() {};

        /**
         * @param script The script to evaluate
         */
        virtual String eval(const String& script) = 0;

        /**
         * @param name Name of the command to create
         * @param callback The script_callback_t function to call when the command is ran
         */
        virtual void createCommand(const String& name, script_callback_string_t callback, script_clientdata_t clientData = NULL) = 0;
        virtual void createCommand(const String& name, script_callback_int_t callback, script_clientdata_t clientData = NULL) = 0;

        /**
         * @brief Link a String to a variable in the interp
         * @param name Name of the variable to link
         * @param var The variable to link to
         */
        virtual void linkVar(const String& name, String& var) = 0;
        virtual void linkVar(const String& name, const String& var) = 0;

        /**
         * @brief Link an int to a variable in the interp
         * @param name Name of the variable to link
         * @param var The variable to link to
         */
        virtual void linkVar(const String& name, int& var) = 0;
        virtual void linkVar(const String& name, const int& var) = 0;

        /**
         * @brief Link an unsigned int to a variable in the interp
         * @param name Name of the variable to link
         * @param var The variable to link to
         */
        virtual void linkVar(const String& name, unsigned int& var) = 0;
        virtual void linkVar(const String& name, const unsigned int& var) = 0;

        /**
         * @brief Link a long to a variable in the interp
         * @param name Name of the variable to link
         * @param var The variable to link to
         */
        virtual void linkVar(const String& name, long& var) = 0;
        virtual void linkVar(const String& name, const long& var) = 0;

        /**
         * @brief Link an unsigned long to a variable in the interp
         * @param name Name of the variable to link
         * @param var The variable to link to
         */
        virtual void linkVar(const String& name, unsigned long& var) = 0;
        virtual void linkVar(const String& name, const unsigned long& var) = 0;

        /**
         * @brief Link a double to a variable in the interp
         * @param name Name of the variable to link
         * @param var The variable to link to
         */
        virtual void linkVar(const String& name, double& var) = 0;
        virtual void linkVar(const String& name, const double& var) = 0;

        /**
         * @brief Link a bool to a variable in the interp
         * @param name Name of the variable to link
         * @param var The variable to link to
         */
        virtual void linkVar(const String& name, bool& var) = 0;
        virtual void linkVar(const String& name, const bool& var) = 0;
};


BDLIB_NS_END
#endif /* _BD_SCRIPTINTERP_H */
