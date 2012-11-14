#ifndef YAMPL_POLLER_H
#define YAMPL_POLLER_H

#include <sys/epoll.h>

#include "yampl/Exceptions.h"

namespace yampl{

class Poller{
  public:
    Poller(){
      if((m_poll = epoll_create(1)) == -1)
	throw ErrnoException("Failed to create epoll handle");
    }

    ~Poller(){
      if(close(m_poll) == -1)
	throw ErrnoException("Failed to close epoll handle");
    }

    void add(int fd, void *data = NULL){
      epoll_event ev;
      ev.events = EPOLLIN;
      ev.data.ptr = data;

      if(epoll_ctl(m_poll, EPOLL_CTL_ADD, fd, &ev) == -1)
	throw ErrnoException("Failed to add fd to epoll handle");
    }

    int poll(void **data, int timeout = 500){
      epoll_event ev;

      while(true){
	switch(epoll_wait(m_poll, &ev, 1, timeout)){
	  case 0:
	    return -1;

	  case -1:
	    if(errno == EINTR){
	      continue;
	    }
	    throw ErrnoException("Failed to wait on epoll handle");

	  default:
	    break;
	}

	break;
      }

      *data = ev.data.ptr;
      return ev.data.fd;
    }

    int poll(){
      void *data;
      return poll(&data);
    }


  private:
    int m_poll;
};

}

#endif
