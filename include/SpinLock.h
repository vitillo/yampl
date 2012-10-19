#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <atomic>

namespace IPC{

class SpinLock{
  public:
    void lock(){
      while(m_flag.test_and_set(std::memory_order_acquire));
    }

    void unlock(){
      m_flag.clear(std::memory_order_release);
    }

  private:
    std::atomic_flag m_flag = ATOMIC_FLAG_INIT;
};

}

#endif
