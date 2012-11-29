#ifndef YAMPL_THREAD_H
#define YAMPL_THREAD_H

#include <pthread.h>
#include <tr1/functional>

#include "yampl/Exceptions.h"

namespace yampl{

class Thread{
  public:
    Thread(const std::tr1::function<void()> &fun) : m_isDestroyable(false), m_fun(new std::tr1::function<void()>(fun)){
      int err = pthread_create(&m_thread, NULL, startWrapper, this);

      if(err){
        throw ErrnoException(err, "Failed to create thread");
      }
    }

    ~Thread(){
      if(!m_isDestroyable)
	throw InvalidOperationException("Thread has to be detached or joined");

      delete m_fun;
    }

    void join(){
      if(pthread_join(m_thread, NULL)){
	throw ErrnoException("Failure to join thread");
      }
      m_isDestroyable = true;
    }

    void detach(){
      if(pthread_detach(m_thread)){
	throw ErrnoException("Failure to detach thread");
      }
      m_isDestroyable = true;
    }

    void cancel(){
      if(pthread_cancel(m_thread)){
	throw ErrnoException("Failure to cancel thread");
      }
    }

  private:
    Thread(const Thread &);
    Thread & operator=(const Thread &);

    static void *startWrapper(void *arg){
      Thread *thread = (Thread *)arg;
      (*thread->m_fun)();
      delete thread->m_fun;
      thread->m_fun = 0;
      return NULL;
    }

    bool m_isDestroyable;
    pthread_t m_thread;
    std::tr1::function<void()> *m_fun;
};

}

#endif
