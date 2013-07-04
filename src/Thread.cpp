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

/* Thread.cpp
 */

#include "Thread.h"
#if defined(WIN32)
# include <process.h>
#endif
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif
#ifdef HAVE_SYS_SELECT_H
# include <sys/select.h>
#endif
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

BDLIB_NS_BEGIN
static threadMainRet_t threadMain(void* object) {
  Thread* thread = (Thread*) object;
  return (threadMainRet_t) thread->run(thread->param);
}

int Thread::start(void* _param) {
  if (!isStarted()) {
    param = _param;
#if defined(USE_PTHREAD)
    /* Initialize the pthread_attr needed for pthread .. */
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    /* ... and set the detached state if we are to be detached */
    if (isDetached())
      pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);  

    /* Attempt to create the thread, and set to started if successfull */
    if (pthread_create(&(this->handle), &attr, threadMain, this) == 0)
      setStarted();
    pthread_attr_destroy(&attr);
/* endif USE_PTHREAD */
#elif defined(WIN32)  
    /* Attempt to create the thread */
    if (this->handle = (HANDLE)_beginthreadex(NULL, 0, threadMain, this, 0, &(this->id)) {
      if (isDetached())
        CloseHandle(this->handle);
      setStarted();
    }
#elif defined(NO_THREADS) /* NO THREAD SUPPORT */
    /* No thread support, pretend the thread started */
    setStarted();
#endif 
  }
  return isStarted();
}

void Thread::detach(void) {
  if (shouldDetach()) {
#if defined(USE_PTHREAD)
    pthread_detach(this->handle);
#elif defined(WIN32)
    CloseHandle(this->handle);
#elif defined(NO_THREADS) /* NO THREAD SUPPORT */
    ; /* nop */
#endif
  }
  setDetached();
}

void* Thread::wait(void) {
  void* _status = NULL;

  if (shouldDetach()) {
#if defined(USE_PTHREAD)
    pthread_join(this->handle, &_status);
#elif defined(WIN32)
    WaitForSingleObject(this->handle, INFINITE);
    GetExitCodeThread(this->handle, (DWORD*) &_status);
    CloseHandle(this->handle);
#endif /* WIN32 */
    setDetached();
  }
  return _status;
}

void Thread::stop(void) {
  if (shouldDetach()) {
#if defined(USE_PTHREAD)
    pthread_cancel(this->handle);
    pthread_detach(this->handle);
#elif defined(WIN32)
    TerminateThread(this->handle, 0);
    CloseHandle(this->handle);
#endif
    setDetached();
  }
}

threadId_t Thread::getThreadId(void) {
  return this->id;
}

threadId_t Thread::getCurrentThreadId(void) {
#if defined(USE_PTHREAD) || defined(NO_THREADS)
  return 0;
#elif defined(WIN32)
  return GetCurrentThreadId();
#endif
}

void Thread::sleep(int delay) {
#if defined(USE_PTHREAD) || defined(NO_THREADS)
  timeval timeout;

  timeout.tv_sec = delay / 1000;
  timeout.tv_usec = (delay * 1000) % 1000000;
  select(0, (fd_set*) NULL, (fd_set*) NULL, (fd_set*) NULL, &timeout);
#elif defined(WIN32)
  ::Sleep(delay);
#endif
}
BDLIB_NS_END
