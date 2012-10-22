#ifndef ISOCKET_H
#define ISOCKET_H

namespace IPC{

class ISocket{
  public:
    virtual ~ISocket(){}

    virtual void send(void *buffer, size_t size, void *hint = 0) = 0;
    virtual size_t recv(void **buffer, size_t size = 0) = 0;

    template <typename T>
    void send(const T *buffer, size_t size, void *hint = 0){
      send((void *)buffer, size, hint);
    }

    template <typename T>
    size_t recv(T **buffer, size_t size = 0){
      return recv((void **)buffer, size);
    }
    
  private:
    ISocket & operator=(const ISocket &);
};

}

#endif
