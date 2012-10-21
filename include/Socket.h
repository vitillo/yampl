#ifndef SOCKET_H
#define SOCKET_H

namespace IPC{

class ISocket{
  public:
    virtual ~ISocket(){}

    virtual void send(const void *buffer, size_t size, void *hint = 0) = 0;
    virtual size_t receive(void **buffer, size_t size = 0) = 0;

    template <typename T>
    void send(const T *buffer, size_t n, void *hint = 0){
      send((void *)buffer, n * sizeof(T), hint);
    }

    template <typename T>
    size_t receive(T **buffer, size_t size = 0){
      return receive((void **)buffer, size);
    }

  private:
    ISocket & operator=(const ISocket &);
};

}

#endif
