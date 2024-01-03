#ifndef YAMPL_THREAD_H
#define YAMPL_THREAD_H

#include <pthread.h>
#include <tr1/functional>

#include "yampl/Exceptions.h"

namespace yampl{

class Thread{
  public:
    Thread(const std::tr1::function<void()> &fun);
    ~Thread() noexcept(false);

    void join();
    void detach();
    void cancel();

    static void disableCancelState();
    static void enableCancelState();

  private:
    Thread(const Thread &);
    Thread & operator=(const Thread &);

    static void *startWrapper(void *arg);

    bool m_isDestroyable;
    pthread_t m_thread;
    std::tr1::function<void()> *m_fun;
};

inline void Thread::disableCancelState(){
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
}

inline void Thread::enableCancelState(){
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
}

inline Thread::Thread(const std::tr1::function<void()> &fun) : m_isDestroyable(false), m_fun(new std::tr1::function<void()>(fun)){
  int err = pthread_create(&m_thread, NULL, startWrapper, this);

  if(err){
    throw ErrnoException(err, "Failed to create thread");
  }
}

inline Thread::~Thread() noexcept(false) {
  if(!m_isDestroyable)
    throw InvalidOperationException("Thread has to be detached or joined");

  delete m_fun;
}

inline void Thread::join(){
  if(pthread_join(m_thread, NULL)){
    throw ErrnoException("Failure to join thread");
  }
  m_isDestroyable = true;
}

inline void Thread::detach(){
  if(pthread_detach(m_thread)){
    throw ErrnoException("Failure to detach thread");
  }
  m_isDestroyable = true;
}

inline void Thread::cancel(){
  pthread_cancel(m_thread);
}

inline void * Thread::startWrapper(void *arg){
  Thread *thread = (Thread *)arg;
  (*thread->m_fun)();
  delete thread->m_fun;
  thread->m_fun = 0;
  return NULL;
}

}

#endif
