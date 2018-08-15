// os-windows.h
//
// OS specific stuff for Windows

/*
  Original code: Copyright (c) 2014 Microsoft Corporation
  Modified code: Copyright (c) 2015 VMware, Inc
  All rights reserved. 

  Written by Marcos K. Aguilera

  MIT License

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation files
  (the ""Software""), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of the Software,
  and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/



//#include <intrin.h>
//#include <tchar.h>
//#include <assert.h>
//#include <string.h>
//#include <stdio.h>

// rtchk is supposed to exit if condition is not satisfied
#define rtchk(cond)  assert(cond)


// these definitions for fetch-and-add, atomic-increment, and atomic-decrement are compiler specific
#define FetchAndAdd32(ptr32, val32) _InterlockedExchangeAdd((long*)ptr32,val32)
#define FetchAndAdd64(ptr64, val64) _InterlockedExchangeAdd64((long*)ptr64,val64)
#define AtomicInc32(ptr32) _InterlockedIncrement((long*)ptr32)
#define AtomicInc64(ptr64) _InterlockedIncrement64((__int64*)ptr64)
#define AtomicDec32(ptr32) _InterlockedDecrement((long*)ptr32)
#define AtomicDec64(ptr64) _InterlockedIncrement64((__int64*)ptr64)
#define CompareSwap32(ptr32,cmp32,val32) _InterlockedCompareExchange(ptr32,val32,cmp32)
#define CompareSwap64(ptr64,cmp64,val64) _InterlockedCompareExchange(ptr64,val64,cmp64)
#define CompareSwapPtr(ptr,cmp,val) _InterlockedCompareExchangePointer(ptr,val,cmp)

// definitions to align a variable in 4-byte or 64-byte boundaries, also compiler specific
#define Align4 __declspec(align(4))
#define Align8 __declspec(align(8))
#define Align16 __declspec(align(16))
#define Align64 __declspec(align(64))

#define Tlocal __declspec(thread)

#define SYNC_TYPE 4
// possible values:
//    1 = use lightweight windows synchronization (SRWLocks, etc)
//    2 = use synchronization internal to the Visual Studio C Run-time Library
//    3 = use regular windows synchronization (CreateMutex/WaitForSingleObject/ReleaseMutex)
//        (no support for condition variables)
//    4 = C++11

#define WINDOWS_SEMAPHORE // uses windows semaphore instead of C++11 semaphores built with mutex and condition_variable

#include "inttypes.h"

#if SYNC_TYPE==1
class RWLock {
  friend class CondVar;
protected:
  SRWLOCK SRWLock;
public:
  RWLock(){
    InitializeSRWLock(&SRWLock);
  }

  void lock(void){ AcquireSRWLockExclusive(&SRWLock); }
  void lockRead(void){ AcquireSRWLockShared(&SRWLock); }
  void unlock(void){ ReleaseSRWLockExclusive(&SRWLock); }
  void unlockRead(void){  ReleaseSRWLockShared(&SRWLock); }
  int trylock(void){ return TryAcquireSRWLockExclusive(&SRWLock); }
  int trylockRead(void){ return TryAcquireSRWLockShared(&SRWLock); }
  // the try functions return true if lock was gotten, false if someone else holds the lock
};
class CondVar {
  CONDITION_VARIABLE CV;
public:
  CondVar(){ InitializeConditionVariable(&CV); }
  void wakeOne(void){ WakeConditionVariable(&CV); }
  void wakeAll(void){ WakeAllConditionVariable(&CV); }

  // Release write lock and sleep atomically.
  // When the thread is woken up, lock is reacquired.
  // Returns true if successful (lock acquired), false if timeout (lock not acquired)
  bool unlockSleepLock(RWLock *lock, int mstimeout){ return SleepConditionVariableSRW(&CV, &lock->SRWLock, mstimeout, 0) != 0; }

  // Release read lock and sleep atomically.
  // When the thread is woken up, lock is reacquired.
  // Returns true if successful (lock acquired), false if timeout (lock not acquired)
  bool unlockReadSleepLockRead(RWLock *lock, int mstimeout){ return SleepConditionVariableSRW(&CV, &lock->SRWLock, mstimeout, CONDITION_VARIABLE_LOCKMODE_SHARED) != 0; }
};
#elif SYNC_TYPE==2
#include <thr/xthreads.h>
#include <chrono>
#include <thread>
#include <concrt.h>

typedef void * _CONCRT_BUFFER;

struct my_LockQueueNode
{
  // Const statics needed for blocking heuristics
  static const unsigned int TicketThreshold        = 2;
  static const unsigned int StateIsBlocked         = 0x00000001;
  static const unsigned int StateIsTicketValid     = 0x00000002;
  static const unsigned int StateIsPreviousBlocked = 0x00000004;
  static const unsigned int MaskBlockedStates      = ~(StateIsBlocked | StateIsPreviousBlocked);
  static const unsigned int NumberOfBooleanStates  = 0x00000003;
  static const unsigned int TicketIncrement        = 1 << NumberOfBooleanStates;

  void *                 m_pContext;
  my_LockQueueNode * volatile  m_pNextNode;
  volatile unsigned int     m_ticketState;
  // Timer handle (valid only for XP; on Vista and above, this handle only used as the indication of whether current node is a timed node.
  HANDLE m_hTimer;
  // The trigger - for timed waits, the unblock mechanism competes with the timer to trigger the thread attempting to acquire the lock.
  // Note, the acquiring thread may fire the trigger itself if the lock is not held - this counts as a virtual 'unblock'
  volatile long m_trigger;
  volatile long m_fTimedNodeInvalid;
};

struct my_critical_section
{
public:
    _CONCRT_BUFFER  _M_activeNode[(4 * sizeof(void *) + 2 * sizeof(unsigned int) + sizeof(_CONCRT_BUFFER) - 1) / sizeof(_CONCRT_BUFFER)];
    void * volatile _M_pHead;
    void * volatile _M_pTail;
};

class RWLock {
  friend class CondVar;
protected:
  Concurrency::critical_section l;
public:
  ~RWLock(){
#ifdef _DEBUG
    my_critical_section *m = (my_critical_section *) &l;
    if (m->_M_pHead){ // lock is held, steal it from whoever has it
      Concurrency::critical_section l2; // the only purpose of l2 is to provide a context
                                        // that we can copy to l, so we can unlock l
      void *save_pContext;
      l2.lock();
      my_critical_section *ptr = (my_critical_section *) &l;
      my_LockQueueNode *pCurrentNode = (my_LockQueueNode*) ptr->_M_pHead;

      my_critical_section *ptr2 = (my_critical_section *) &l2;
      my_LockQueueNode *pCurrentNode2 = (my_LockQueueNode*) ptr2->_M_pHead;

      // save original context
      save_pContext = pCurrentNode->m_pContext;
      // copy to context to one owned by our lock l2
      pCurrentNode->m_pContext = pCurrentNode2->m_pContext;
      // now we can unlock
      l.unlock();
      // restore original context
      pCurrentNode->m_pContext = save_pContext;
      l2.unlock();
    }
#endif
  }

  void lock(void){ l.lock(); }
  void lockRead(void){ lock(); }
  void unlock(void){ l.unlock(); }
  void unlockRead(void){ unlock(); }
  int trylock(void){ return l.try_lock(); }
  int trylockRead(void){ return trylock(); }
  // the try functions return true if lock was gotten, false if someone else holds the lock
};
class CondVar {
   Concurrency::details::_Condition_variable cv;
public:
  void wakeOne(void){ cv.notify_one(); }
  void wakeAll(void){ cv.notify_all(); }

  // Release write lock and sleep atomically.
  // When the thread is woken up, lock is reacquired.
  // returns true if successful, false if timeout
  bool unlockSleepLock(RWLock *lock, int mstimeout){ 
    if (mstimeout!=INFINITE) return cv.wait_for(lock->l, mstimeout);
    else {
      cv.wait(lock->l);
      return true;
    }
  }

  // Release read lock and sleep atomically.
  // When the thread is woken up, lock is reacquired.
  bool unlockReadSleepLockRead(RWLock *lock, int mstimeout){ return unlockSleepLock(lock, mstimeout); }
};
//
//
//class RWLock {
//  friend class CondVar;
//protected:
//  _Mtx_t m;
//public:
//  RWLock(){ _Mtx_init(&m, 0); }
//  ~RWLock(){
//    _Mtx_internal_imp_t *mm = (_Mtx_internal_imp_t*) m;
//    if (mm->count > 0){
//      Concurrency::critical_section cs;
//      cs.lock
//      mm->thread_id = GetCurrentThreadId(); // steal lock
//      while (mm->count > 0) _Mtx_unlock(&m);
//    }
//    _Mtx_destroy(&m); }
//  void lock(void){ _Mtx_lock(&m); }
//  void lockRead(void){ lock(); }
//  void unlock(void){ _Mtx_unlock(&m); }
//  void unlockRead(void){ unlock(); }
//  int trylock(void){ return _Mtx_trylock(&m); }
//  int trylockRead(void){ return trylock(); }
//  // the try functions return true if lock was gotten, false if someone else holds the lock
//};
//class CondVar {
//  _Cnd_t cv;
//public:
//  CondVar(){ _Cnd_init(&cv); }
//  ~CondVar(){ _Cnd_destroy(&cv); }
//
//  void wakeOne(void){ _Cnd_signal(&cv); }
//  void wakeAll(void){ _Cnd_broadcast(&cv); }
//
//  // Release write lock and sleep atomically.
//  // When the thread is woken up, lock is reacquired.
//  // returns true if successful, false if timeout
//  bool unlockSleepLock(RWLock *lock, int mstimeout){ 
//    if (mstimeout!=INFINITE){
//      stdext::threads::xtime _Tgt = _To_xtime(std::chrono::milliseconds(mstimeout));
//      return _Cnd_timedwait(&cv,&lock->m, &_Tgt) == _Thrd_success;
//    }
//    else return _Cnd_wait(&cv, &lock->m) == _Thrd_success;
//  }
//
//  // Release read lock and sleep atomically.
//  // When the thread is woken up, lock is reacquired.
//  bool unlockReadSleepLockRead(RWLock *lock, int mstimeout){ return unlockSleepLock(lock, mstimeout); }
//};

//#include <condition_variable>
//#include <concrt.h>
//
//class RWLock {
//  friend class CondVar;
//protected:
//  std::mutex *lo;
//public:
//  RWLock(){ 
//    lo = new std::mutex;
//    ul = std::unique_lock<std::mutex>(*lo);
//    ul.unlock();
//  }
//  ~RWLock(){
//    if (ul.owns_lock()) ul
//    ul.release();
//    (lolo._Mtx
//
//    if ((*(_Mtx_t*)&lo)->count == 0) delete lo;
//    // else trying to destroy a lock held by someone. Leave lock allocated
//    // otherwise the CRT will thrown an exception
//  }
//  void lock(void){ ul.lock(); }
//  void lockRead(void){ ul.lock(); }
//  void unlock(void){ ul.unlock(); }
//  void unlockRead(void){ ul.unlock(); }
//  int trylock(void){ return ul.try_lock(); }
//  int trylockRead(void){ return ul.try_lock(); }
//  // the try functions return true if lock was gotten, false if someone else holds the lock
//};
//class CondVar {
//  std::condition_variable CV;
//public:
//  void wakeOne(void){ CV.notify_one(); }
//  void wakeAll(void){ CV.notify_all(); }
//
//  // Release write lock and sleep atomically.
//  // When the thread is woken up, lock is reacquired.
//  // returns true if successful, false if timeout
//  bool unlockSleepLock(RWLock *lock, int mstimeout){ 
//    std::unique_lock<std::mutex> ul(*lock->lo);
//    if (mstimeout!=INFINITE)
//      return CV.wait_for(ul, std::chrono::milliseconds(mstimeout))==std::cv_status::no_timeout;
//    else {
//      CV.wait(ul);
//      return true;
//    }
//  }
//
//  // Release read lock and sleep atomically.
//  // When the thread is woken up, lock is reacquired.
//  bool unlockReadSleepLockRead(RWLock *lock, int mstimeout){ return unlockSleepLock(lock, mstimeout); }
//};
#elif SYNC_TYPE==3
class RWLock {
  //friend class CondVar;
protected:
  HANDLE mutex;
public:
  RWLock(){ mutex = CreateMutex(0, 0, 0); }
  ~RWLock(){ CloseHandle(mutex); }
  void lock(void){ while (WaitForSingleObject(mutex,INFINITE) != WAIT_OBJECT_0) ; }
  void lockRead(void){ lock(); }
  void unlock(void){ ReleaseMutex(mutex); }
  void unlockRead(void){  unlock(); }
  int trylock(void){ return WaitForSingleObject(mutex,0)==0; }
  int trylockRead(void){ return trylock(); }
  // the try functions return true if lock was gotten, false if someone else holds the lock
};
#elif SYNC_TYPE==4
#include <mutex>
class RWLock {
  //friend class CondVar;
protected:
  std::mutex m;
public:
  RWLock(){ }
  ~RWLock(){ }
  void lock(void){ m.lock(); }
  void lockRead(void){ lock(); }
  void unlock(void){ m.unlock(); }
  void unlockRead(void){  unlock(); }
  int trylock(void){ return m.try_lock(); }
  int trylockRead(void){ return trylock(); }
  // the try functions return true if lock was gotten, false if someone else holds the lock
};
#endif

#ifdef WINDOWS_SEMAPHORE
class Semaphore {
  static const int MAXSEMAPHOREVALUE=2147483647; // largest LONG
private:
  HANDLE SemaphoreObject;
public:
  Semaphore(int initialValue=0){
    SemaphoreObject = CreateSemaphore(0, initialValue, MAXSEMAPHOREVALUE, 0);
  }
  ~Semaphore(){ CloseHandle(SemaphoreObject); }

  // returns true if timeout expired, false if semaphore has been signaled
  // if msTimeout=INFINITE then wait forever
  bool wait(int msTimeout){
    u32 res;
    res = WaitForSingleObject(SemaphoreObject, msTimeout);
    if (res == WAIT_OBJECT_0) return false;
    if (res == WAIT_TIMEOUT) return true;
    rtchk(0);
    return false;
  }

  void signal(void){
    int res = ReleaseSemaphore(SemaphoreObject, 1, 0); rtchk(res);
  }
};
#else
// Semaphore built with condition variable and mutex
#include <condition_variable>
class Semaphore {
  static const int MAXSEMAPHOREVALUE=2147483647; // largest LONG
private:
  Align4 u32 value;
  std::mutex m;
  std::condition_variable cv;
public:
  Semaphore(int initialValue=0){
    value = initialValue;
  }

  // returns true if timeout expired, false if semaphore has been signaled
  // if msTimeout=INFINITE then wait forever
  bool wait(int msTimeout){
    std::unique_lock<std::mutex> uniquelck(m);
    while (value==0){
      if (msTimeout != INFINITE){
        std::_Cv_status s;
        s = cv.wait_for(uniquelck, std::chrono::milliseconds(msTimeout));
        if (s == std::_Cv_status::timeout) return true;
      }
      else
        cv.wait(uniquelck);
    }
    AtomicDec32(&value);
    return false;
  }

  void signal(void){
    std::unique_lock<std::mutex> uniquelck(m);
    AtomicInc32(&value);
    cv.notify_one();
  }
};
#endif

//#include <concrtrm.h>
//#include <ppl.h>

#include <concrt.h>

#define EventSync EventSyncTwo  // use windows events

// event synchronization based on concurrency::set/wait
class EventSyncZero {
private:
  concurrency::event e;
public:
  void set(){ e.set(); }
  void reset(){ e.reset(); }
  int wait(unsigned timeout = concurrency::COOPERATIVE_TIMEOUT_INFINITE){  // returns 0 if wait satisfied, non-0 otherwise
    return (int) e.wait(timeout); 
  }
  //int wait(unsigned timeout = concurrency::COOPERATIVE_TIMEOUT_INFINITE){  // returns 0 if wait satisfied, non-0 otherwise
  //  concurrency::event *eptr = &e;
  //  return concurrency::event::wait_for_multiple(&eptr, 1, true, concurrency::COOPERATIVE_TIMEOUT_INFINITE);
  //}
};

// event synchronization based on concurrency::set/wait_for_multiple
class EventSyncZeroAlt {
private:
  concurrency::event e;
public:
  void set(){ e.set(); }
  void reset(){ e.reset(); }
  int wait(unsigned timeout = concurrency::COOPERATIVE_TIMEOUT_INFINITE){  // returns 0 if wait satisfied, non-0 otherwise
    concurrency::event *eptr = &e;
    size_t status;
    bool waitinfinite;
    if (timeout == concurrency::COOPERATIVE_TIMEOUT_INFINITE){
      waitinfinite = true;
      timeout = 1000000; // avoid calling wait_for_multiple with infinity to prevent spinning in the library
    } else waitinfinite = false;
    do {
      status = concurrency::event::wait_for_multiple(&eptr, 1, true, timeout);
    } while (waitinfinite && status == concurrency::COOPERATIVE_WAIT_TIMEOUT);
    return status == concurrency::COOPERATIVE_WAIT_TIMEOUT;
  }
};

class EventSyncOne {
private:
  SRWLOCK SRWLock;
public:
  EventSyncOne(){
    InitializeSRWLock(&SRWLock);
    AcquireSRWLockExclusive(&SRWLock);
  }
  void set(){ ReleaseSRWLockExclusive(&SRWLock); }
  int wait(void){  // returns 0 if wait satisfied, non-0 otherwise
    AcquireSRWLockExclusive(&SRWLock);
    return 0;
  }
};

class EventSyncTwo {
private:
  HANDLE Event;
public:
  EventSyncTwo(){
    Event = CreateEvent(0, true, false, 0);
  }
  ~EventSyncTwo(){
    CloseHandle(Event);
  }
  void set(){ int res = SetEvent(Event); rtchk(res); }
  void reset(){ int res = ResetEvent(Event); rtchk(res); }
  int wait(unsigned timeout = INFINITE){  // returns 0 if wait satisfied, non-0 otherwise
    int res;
    res = WaitForSingleObject(Event, timeout);
    return res;
  }
};

class EventSyncThree {
private:
  HANDLE Semaphore;
public:
  EventSyncThree(){
    Semaphore = CreateSemaphore(0, 0, 10, 0);
  }
  ~EventSyncThree(){
    CloseHandle(Semaphore);
  }
  void set(){ int res = ReleaseSemaphore(Semaphore, 1, 0); rtchk(res); }
  int wait(void){  // returns 0 if wait satisfied, non-0 otherwise
    int res;
    res = WaitForSingleObject(Semaphore, INFINITE); rtchk(res==0);
    return 0;
  }
};

class EventSyncFour {
private:
  CONDITION_VARIABLE cv;
  CRITICAL_SECTION cs;
  int flag;
public:
  EventSyncFour(){
    InitializeConditionVariable(&cv);
    InitializeCriticalSection(&cs);
    flag = 0;
  }
  void set(){ 
    flag = 1;
    WakeConditionVariable(&cv);
  }
  int wait(void){  // returns 0 if wait satisfied, non-0 otherwise
    int res;
    EnterCriticalSection(&cs);
    while (flag == 0){
      res = SleepConditionVariableCS(&cv, &cs, INFINITE);
      rtchk(res);
    }
    return 0;
  }
};

typedef HANDLE OSThread_t;
typedef DWORD OSThread_return_t;
#define OSTHREAD_FUNC WINAPI OSThread_return_t

// The start routine of a thread should be as follows:
//  OSTHREAD_FUNC name(void *arg){
//    ..
//    return (OSThread_return_t) numeric value;
//  }

// create a new thread starting func(arg)
// Sets *thread to thread id
// Returns 0 if ok, non-zero if error
int OSCreateThread(OSThread_t *thread, OSThread_return_t (*func)(void*), void *arg);

// wait for thread to end, storing its return value in *res
// Returns 0 if ok, non-zero if error
int OSWaitThread(OSThread_t thread, void **res);

#endif

