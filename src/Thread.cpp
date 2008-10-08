/* Thread.cpp
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

#ifndef lint
static const char rcsid[] = "$Id$";
#endif /* lint */


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

int Thread::start(void* param) {
  if (!isStarted()) {
    this->param = param;
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
  void* status = NULL;

  if (shouldDetach()) {
#if defined(USE_PTHREAD)
    pthread_join(this->handle, &status);
#elif defined(WIN32)
    WaitForSingleObject(this->handle, INFINITE);
    GetExitCodeThread(this->handle, (DWORD*) &status);
    CloseHandle(this->handle);
#endif /* WIN32 */
    setDetached();
  }
  return status;
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
