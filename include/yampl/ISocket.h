#ifndef YAMPL_ISOCKET_H
#define YAMPL_ISOCKET_H

#include "plugin/IObject.h"
#include "utils/Globals.h"

#include <unistd.h>
#include <assert.h>
#include <string>

namespace yampl
{
//    extern std::string DEFAULT_ID;

    class ISocket
    {
        public:
            virtual ~ISocket() noexcept(false) {}

            virtual void send();
            virtual void send(void *buffer, size_t size, void *hint = 0);
            virtual void sendTo(const std::string &peerId, void *buffer, size_t size, void *hint = 0);

            virtual ssize_t recv(void *&buffer, size_t size, std::string &peerId = DEFAULT_ID);
            virtual ssize_t recv(void *&buffer, std::string &peerId = DEFAULT_ID);

            virtual ssize_t tryRecv(void *&buffer, size_t size, long timeout = 0, std::string &peerId = DEFAULT_ID);
            virtual ssize_t tryRecv(void *&buffer, long timeout = 0, std::string &peerId = DEFAULT_ID);

            // send wrappers
            template <typename T>
            void send(T *buffer, size_t n, void *hint = 0){
              assert(__is_pod(T) || (__has_trivial_copy(T) && __has_trivial_assign(T) && __has_trivial_destructor(T)));
              send((void *)buffer, n * sizeof(T), hint);
            }

            template <typename T, int N>
            void send(T (&buffer)[N], void *hint = 0){
              assert(__is_pod(T) || (__has_trivial_copy(T) && __has_trivial_assign(T) && __has_trivial_destructor(T)));
              send((void *)&buffer[0], sizeof(buffer), hint);
            }

            template <typename T>
            void send(const T &value, void *hint = 0){
              assert(__is_pod(T) || (__has_trivial_copy(T) && __has_trivial_assign(T) && __has_trivial_destructor(T)));
              send((void *)&value, sizeof(T), hint);
            }

            void send(const std::string &msg, void *hint = 0){
              send(msg.c_str(), msg.size() + 1, hint);
            }

            template <typename T>
            void sendTo(const std::string &peerId, T *buffer, size_t n, void *hint = 0){
              assert(__is_pod(T) || (__has_trivial_copy(T) && __has_trivial_assign(T) && __has_trivial_destructor(T)));
              sendTo(peerId, (void *)buffer, n * sizeof(T), hint);
            }

            template <typename T, int N>
            void sendTo(const std::string &peerId, T (&buffer)[N], void *hint = 0){
              assert(__is_pod(T) || (__has_trivial_copy(T) && __has_trivial_assign(T) && __has_trivial_destructor(T)));
              sendTo(peerId, (void *)&buffer[0], sizeof(buffer), hint);
            }

            template <typename T>
            void sendTo(const std::string &peerId, const T &value, void *hint = 0){
              assert(__is_pod(T) || (__has_trivial_copy(T) && __has_trivial_assign(T) && __has_trivial_destructor(T)));
              sendTo(peerId, (void *)&value, sizeof(T), hint);
            }

            void sendTo(const std::string &peerId, const std::string &msg, void *hint = 0){
              sendTo(peerId, msg.c_str(), msg.size() + 1, hint);
            }

            // recv wrappers
            template <typename T>
            ssize_t recv(T *&buffer, size_t n, std::string &peerId = DEFAULT_ID){
              assert(__is_pod(T) || (__has_trivial_copy(T) && __has_trivial_assign(T) && __has_trivial_destructor(T)));
              void **tmp = (void **)&buffer;
              return recv(*tmp, n * sizeof(T), peerId);
            }

            template <typename T>
            ssize_t recv(T *&buffer, std::string &peerId = DEFAULT_ID){
              assert(__is_pod(T) || (__has_trivial_copy(T) && __has_trivial_assign(T) && __has_trivial_destructor(T)));
              void **tmp = (void **) &buffer;
              return recv(*tmp, peerId);
            }

            template <typename T, int N>
            ssize_t recv(T (&buffer)[N], std::string &peerId = DEFAULT_ID){
              assert(__is_pod(T) || (__has_trivial_copy(T) && __has_trivial_assign(T) && __has_trivial_destructor(T)));
              T *ptr = &buffer[0];
              return recv(ptr, sizeof(buffer), peerId);
            }

            template <typename T>
            T recv(std::string &peerId = DEFAULT_ID){
              assert(__is_pod(T) || (__has_trivial_copy(T) && __has_trivial_assign(T) && __has_trivial_destructor(T)));
              T tmp, *ptr = &tmp;
              recv(ptr, sizeof(T), peerId);
              return tmp;
            }

            // tryRecv wrapper
            template <typename T>
            ssize_t tryRecv(T *&buffer, size_t n, long timeout = 0, std::string &peerId = DEFAULT_ID){
              assert(__is_pod(T) || (__has_trivial_copy(T) && __has_trivial_assign(T) && __has_trivial_destructor(T)));
              void **tmp = (void **)&buffer;
              return tryRecv(*tmp, n * sizeof(T), timeout, peerId);
            }

            template <typename T>
            ssize_t tryRecv(T *&buffer, long timeout = 0, std::string &peerId = DEFAULT_ID){
              assert(__is_pod(T) || (__has_trivial_copy(T) && __has_trivial_assign(T) && __has_trivial_destructor(T)));
              void **tmp = (void **) &buffer;
              return tryRecv(*tmp, timeout, peerId);
            }

            template <typename T, int N>
            ssize_t tryRecv(T (&buffer)[N], long timeout = 0, std::string &peerId = DEFAULT_ID){
              assert(__is_pod(T) || (__has_trivial_copy(T) && __has_trivial_assign(T) && __has_trivial_destructor(T)));
              T *ptr = &buffer[0];
              return tryRecv(ptr, sizeof(buffer), timeout, peerId);
            }

            template <typename T>
            T & tryRecv(long timeout = 0, std::string &peerId = DEFAULT_ID){
              assert(__is_pod(T) || (__has_trivial_copy(T) && __has_trivial_assign(T) && __has_trivial_destructor(T)));
              T tmp, *ptr = &tmp;
              recv(ptr, sizeof(T), timeout, peerId);
              return tmp;
            }

            struct SendArgs{
              public:
            SendArgs(void *buffer, size_t size) : buffer(buffer), size(size), peerId(0), hint(0), custom(0), overrideSemantics(false){}

            void *buffer;
            size_t size;
            const std::string *peerId;
            void *hint;
            void *custom;
            bool overrideSemantics;
            };

            struct RecvArgs{
              public:
            RecvArgs(void **buffer, size_t size) : buffer(buffer), size(size), allocate(false), timeout(-1), peerId(0), peerIdOut(0), custom(0){}
            RecvArgs(void **buffer) : buffer(buffer), size(0), allocate(true), timeout(-1), peerId(0), peerIdOut(0), custom(0){}

            void **buffer;
            size_t size;
            bool allocate;
            long timeout;
            const std::string *peerId;
            std::string *peerIdOut;
            void *custom;
            };

            virtual void send(SendArgs &args) = 0;
            virtual ssize_t recv(RecvArgs &args) = 0;

        private:
            ISocket& operator=(const ISocket &);
    };

    inline void ISocket::send() { // used for synchronization only
          char sync = ' ';
          SendArgs args((void *)&sync, 1);
          args.overrideSemantics = true;
          send(args);
    }

    inline void ISocket::send(void *buffer, size_t size, void *hint){
      SendArgs args(buffer, size);
      args.hint = hint;
      send(args);
    }

    inline void ISocket::sendTo(const std::string& peerId, void *buffer, size_t size, void *hint){
      SendArgs args(buffer, size);
      args.peerId = &peerId;
      args.hint = hint;
      send(args);
    }

    inline ssize_t ISocket::recv(void *&buffer, size_t size, std::string &peerId){
      RecvArgs args(&buffer, size);
      args.peerIdOut = &peerId;
      return recv(args);
    }

    inline ssize_t ISocket::recv(void *&buffer, std::string &peerId){
      RecvArgs args(&buffer);
      args.peerIdOut = &peerId;
      return recv(args);
    }

    inline ssize_t ISocket::tryRecv(void *&buffer, size_t size, long timeout, std::string &peerId){
      RecvArgs args(&buffer, size);
      args.timeout = timeout;
      args.peerIdOut = &peerId;
      return recv(args);
    }

    inline ssize_t ISocket::tryRecv(void *&buffer, long timeout, std::string &peerId){
      RecvArgs args(&buffer);
      args.timeout = timeout;
      args.peerIdOut = &peerId;
      return recv(args);
    }
}

#endif
