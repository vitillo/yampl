#ifndef IPC_SPINLOCK_H
#define IPC_SPINLOCK_H

namespace IPC{

class SpinLock{
  public:
    SpinLock() : m_flag(false){}

    void lock(){
      while(__sync_lock_test_and_set(&m_flag, 1))
	while(m_flag);
    }

    void unlock(){
      __sync_lock_release(&m_flag);
    }


  private:
    SpinLock(const SpinLock&);
    SpinLock & operator=(const SpinLock &);

    volatile bool m_flag;
};

}

#endif
