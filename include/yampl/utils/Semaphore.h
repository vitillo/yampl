#ifndef YAMPL_SEMAPHORE_H
#define YAMPL_SEMAPHORE_H

#include "yampl/utils/Futex.h"

namespace yampl{

class Semaphore{
  public:
    Semaphore(int *sem): m_sem(sem), m_futex(sem){}

    void up(int n){
      if(__sync_fetch_and_add(m_sem, n) < 0){
	m_futex.wake(1);
      }
    }

    void down(int n, long timeout = -1){
      int tmp; 

      //TODO: semaphore has to return when timeout is readed
      //even though *m_sem == tmp
      if((tmp = __sync_sub_and_fetch(m_sem, n)) < 0){
	// TODO: investigate spurious wakeups
	while(*m_sem < 0){
	  m_futex.wait(tmp, timeout);
	}
      }
    }

    int value(){
      return *m_sem;
    }

  private:
    volatile int *m_sem;
    Futex m_futex;
};

}

#endif
