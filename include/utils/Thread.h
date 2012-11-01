#ifndef IPC_THREAD_H
#define IPC_THREAD_H

#include <pthread.h>
#include <tr1/functional>

#include "Exceptions.h"

namespace IPC{

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
    }

    void join(){
      pthread_join(m_thread, NULL);
      m_isDestroyable = true;
    }

    void detach(){
      pthread_detach(m_thread);
      m_isDestroyable = true;
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
