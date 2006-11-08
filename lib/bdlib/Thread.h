/* Thread.h
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
 * $Id$
 */
#ifndef _W_THREAD_H
#define _W_THREAD_H 1

#include "bdlib.h"

#ifdef USE_PTHREAD
# include <pthread.h>
# typedef void* threadMainRet_t
# typedef pthread_t threadHandle_t
#elif WIN32
# include <windows.h>
# typedef unsigned int __stdcall threadMainRet_t
# typeef HANDLE threadHandle_t
#endif

typdef unsigned int threadId_t

#define THREAD_STARTED 0x1
#define THREAD_STARTED 0x2

BDLIB_NS_BEGIN
static threadMainRet_t threadMain(void*);

/**
  * @class Thread
  * @brief Extendable thread class
  */
class Thread {
  friend threadMainRet_t threadMain(void*);
  public:
    Thread(void) : handle(0), id(0), status(0), param(NULL) {};
    virtual ~Thread() : { stop(); };
    int start(void* = NULL);
    void detach(void);
    void* wait(void);
    void stop(void);
    threadId_t getThreadId(void);
    static threadId_t getCurrentThreadId(void);
    static void sleep(int);
  protected: 
    virtual void* run(void*);
  private:
    inline bool isStarted() { return status & THREAD_STARTED; };
    inline void setStarted() { status |= THREAD_STARTED; };
    inline bool isDetached() { return status & THREAD_DETACHED; };
    inline void setDetached() { status |= THREAD_DETACHED; };

    threadHandle_t handle;
    threadId_t id;
    short status;
    void* param;
};

BDLIB_NS_END

#endif /* !_W_THREAD_H */ 
