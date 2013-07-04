/*
 * Copyright (c) 2006-2013, Bryan Drewery <bryan@shatow.net>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Thread.h
 */

#ifndef _BD_THREAD_H
#define _BD_THREAD_H 1

#include "bdlib.h"

#if !defined(USE_PTHREAD) && !defined(WIN32)
# define NO_THREADS 1
#endif

#if defined(USE_PTHREAD)
# include <pthread.h>
typedef void* threadMainRet_t;
typedef pthread_t threadHandle_t;
#elif defined(WIN32)
# include <windows.h>
typedef unsigned int __stdcall threadMainRet_t;
typedef HANDLE threadHandle_t;
#elif defined(NO_THREADS) /* NO THREAD SUPPORT */
typedef void* threadMainRet_t;
typedef short threadHandle_t;
#endif

typedef unsigned int threadId_t;

BDLIB_NS_BEGIN

#define THREAD_STARTED  0x1
#define THREAD_DETACHED 0x2

static threadMainRet_t threadMain(void*);

/**
  * @class Thread
  * @brief Extendable thread class
  */
class Thread {
  private:
    threadHandle_t handle;
    threadId_t id;
    short status;
    void* param;

  friend threadMainRet_t threadMain(void*);
  public:
    Thread(void) : handle(0), id(0), status(0), param(NULL) {};
    virtual ~Thread() { stop(); };
    int start(void* = NULL);
    void detach(void);
    void* wait(void);
    void stop(void);
    threadId_t getThreadId(void);
    static threadId_t getCurrentThreadId(void);
    static void sleep(int);
  protected: 
    virtual void* run(void*)=0;
  private:
    inline bool isStarted() { return status & THREAD_STARTED; };
    inline void setStarted() { status |= THREAD_STARTED; };
    inline bool isDetached() { return status & THREAD_DETACHED; };
    inline void setDetached() { status |= THREAD_DETACHED; };
    /**
     * @brief Is the thread started and not detached?
     * If status == THREAD_STARTED (1), then the THREAD_DETACHED bit is not set, 
     * so this is equivalent to: isStarted() && !isDetached()
     */
    inline bool shouldDetach() { return status == THREAD_STARTED; };
};

BDLIB_NS_END

#endif /* _BD_THREAD_H */ 
