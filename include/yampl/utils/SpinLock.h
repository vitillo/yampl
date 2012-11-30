#ifndef YAMPL_SPINLOCK_H
#define YAMPL_SPINLOCK_H

namespace yampl{

class SpinLock{
  public:
    SpinLock();

    void lock();
    void unlock();

  private:
    SpinLock(const SpinLock&);
    SpinLock & operator=(const SpinLock &);

    volatile bool m_flag;
};

inline SpinLock::SpinLock() : m_flag(false){
}

inline void SpinLock::lock(){
  while(__sync_lock_test_and_set(&m_flag, 1)){
    while(m_flag);
  }
}

inline void SpinLock::unlock(){
  __sync_lock_release(&m_flag);
}

}

#endif
