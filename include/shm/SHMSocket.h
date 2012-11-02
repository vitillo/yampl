#ifndef IPC_SHM_SOCKET_H
#define IPC_SHM_SOCKET_H

#include <string>

#include "ISocket.h"
#include "ISocketFactory.h"
#include "Channel.h"
#include "utils/SpinLock.h"

namespace IPC{
  namespace shm{

    class SocketBase : public ISocket{
      public:
	virtual ~SocketBase();

	virtual void send(void *buffer, size_t size, void *hint = 0);
	virtual size_t recv(void **buffer, size_t size = 0);

      protected:
	SocketBase(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *));

      private:
	SocketBase(const SocketBase &);
	SocketBase & operator=(SocketBase &);

	size_t m_size;
	bool m_doUnlink;
	int m_fd;
	std::string m_name;

	void *m_buffer;
	bool *m_flag;
    };

  }
}

#endif
