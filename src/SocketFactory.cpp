#include "yampl/pipe/SocketFactory.h"
#include "yampl/SocketFactory.h"

#include <cstdlib>
#include <scy/sharedlibrary.h>

using namespace scy;
using namespace scy::pluga;

namespace yampl{

std::string DEFAULT_ID = "";

SocketFactory::SocketFactory()
{
//  m_zmqFactory = new zeromq::SocketFactory();
//  m_pipeFactory = new pipe::SocketFactory();
//  m_shmFactory = new shm::SocketFactory();
}

SocketFactory::~SocketFactory(){
//  delete m_zmqFactory;
//  delete m_pipeFactory;
//  delete m_shmFactory;
}

ISocket *SocketFactory::createClientSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name)
{
    SharedLibrary yampl_lib;
    PluginDetails* info;
    ISocket* socket;

    if (channel.context == LOCAL_PIPE)
    {
        yampl_lib.open("/usr/local/lib/yampl/plugins/yampl-pipe/libyampl-pipe.yam");
        // Parse header
        yampl_lib.sym("exports", reinterpret_cast<void**>(&info));
        auto socket_factory = reinterpret_cast<ISocketFactory*>(info->initializeFunc());
        socket = socket_factory->createClientSocket(channel, semantics, deallocator);
    }
    else if (channel.context == LOCAL_SHM)
    {
        yampl_lib.open("/usr/local/lib/yampl/plugins/yampl-shm/libyampl-shm.yam");
        // Parse header
        yampl_lib.sym("exports", reinterpret_cast<void**>(&info));
        auto socket_factory = reinterpret_cast<ISocketFactory*>(info->initializeFunc());
        socket = socket_factory->createClientSocket(channel, semantics, deallocator);
    }
    else
    {
        yampl_lib.open("/usr/local/lib/yampl/plugins/yampl-zmq/libyampl-zmq.yam");
        // Parse header
        yampl_lib.sym("exports", reinterpret_cast<void**>(&info));
        auto socket_factory = reinterpret_cast<ISocketFactory*>(info->initializeFunc());
        socket = socket_factory->createClientSocket(channel, semantics, deallocator, name);
    }

    factories.push_back(yampl_lib);
    return socket;
//  if(channel.context == LOCAL_PIPE){
//    return m_pipeFactory->createClientSocket(channel, semantics, deallocator);
//  }else if(channel.context == LOCAL_SHM){
//    return m_shmFactory->createClientSocket(channel, semantics, deallocator);
//  }else{
//    return m_zmqFactory->createClientSocket(channel, semantics, deallocator,name);
//  }
}

ISocket *SocketFactory::createServerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)) {
    SharedLibrary yampl_lib;
    PluginDetails *info;
    ISocket *socket;

    if (channel.context == LOCAL_PIPE) {
        yampl_lib.open("/usr/local/lib/yampl/plugins/yampl-pipe/libyampl-pipe.yam");
        // Parse header
        yampl_lib.sym("exports", reinterpret_cast<void **>(&info));
        auto socket_factory = reinterpret_cast<ISocketFactory *>(info->initializeFunc());
        socket = socket_factory->createServerSocket(channel, semantics, deallocator);
    } else if (channel.context == LOCAL_SHM) {
        yampl_lib.open("/usr/local/lib/yampl/plugins/yampl-shm/libyampl-shm.yam");
        // Parse header
        yampl_lib.sym("exports", reinterpret_cast<void **>(&info));
        auto socket_factory = reinterpret_cast<ISocketFactory *>(info->initializeFunc());
        socket = socket_factory->createServerSocket(channel, semantics, deallocator);
    } else {
        yampl_lib.open("/usr/local/lib/yampl/plugins/yampl-zmq/libyampl-zmq.yam");
        // Parse header
        yampl_lib.sym("exports", reinterpret_cast<void **>(&info));
        auto socket_factory = reinterpret_cast<ISocketFactory *>(info->initializeFunc());
        socket = socket_factory->createServerSocket(channel, semantics, deallocator);
    }

    factories.push_back(yampl_lib);
    return socket;
//  if(channel.context == LOCAL_PIPE){
//    return m_pipeFactory->createServerSocket(channel, semantics, deallocator);
//  }else if(channel.context == LOCAL_SHM){
//    return m_shmFactory->createServerSocket(channel, semantics, deallocator);
//  }else{
//    return m_zmqFactory->createServerSocket(channel, semantics, deallocator);
//  }
}
}
