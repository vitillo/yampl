#ifndef YAMPL_POLLER_H
#define YAMPL_POLLER_H

#include <sys/epoll.h>

#include "yampl/Exceptions.h"

namespace yampl{

class Poller{
  public:
    Poller();
    ~Poller();

    int poll(int timeout = -1);
    int poll(void **data, int timeout = -1);
    void add(int fd, void *data = NULL);

  private:
    Poller(const Poller &);
    Poller & operator=(const Poller &);

    int m_poll;
};

inline Poller::Poller(){
  if((m_poll = epoll_create(1)) == -1){
    throw ErrnoException("Failed to create epoll handle");
  }
}

inline Poller::~Poller(){
  if(close(m_poll) == -1){
    throw ErrnoException("Failed to close epoll handle");
  }
}

inline int Poller::poll(int timeout){
  void *data;
  return poll(&data, timeout);
}

inline int Poller::poll(void **data, int timeout){
  epoll_event ev;

  while(true){
    switch(epoll_wait(m_poll, &ev, 1, timeout)){
      case 0:
	return -1;

      case -1:
	if(errno == EINTR){
	  continue;
	}

      default:
	break;
    }

    break;
  }

  *data = ev.data.ptr;
  return ev.data.fd;
}

inline void Poller::add(int fd, void *data){
  epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.ptr = data;

  if(epoll_ctl(m_poll, EPOLL_CTL_ADD, fd, &ev) == -1){
    throw ErrnoException("Failed to add fd to epoll handle");
  }
}

}

#endif
