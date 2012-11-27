#ifndef YAMPL_FUTEX_H
#define YAMPL_FUTEX_H

#include <linux/futex.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>

namespace yampl{

class Futex{
  public:
    Futex(int *addr) : m_addr(addr){}

    bool wait(int expected, long timeout){
      struct timespec t;

      t.tv_nsec = (timeout % 1000) * 1000000;
      t.tv_sec = timeout / 1000;

      if(syscall(SYS_futex, m_addr, FUTEX_WAIT, expected, (timeout == -1 ? NULL : &t), NULL, 0) == -1){
	if(errno == EWOULDBLOCK || errno == EINTR || errno == ETIMEDOUT){
	  return false;
	}else{
	  throw ErrnoException("Failed to wait on futex");
	}
      }

      return true;
    }

    int wake(int n){
      int wokenUp = 0;

      if((wokenUp = syscall(SYS_futex, m_addr, FUTEX_WAKE, n, NULL, NULL, 0)) == -1)
	throw ErrnoException("Failed to wake on futex");

      return wokenUp;
    }

  private:
    int *m_addr;
};

}

#endif
