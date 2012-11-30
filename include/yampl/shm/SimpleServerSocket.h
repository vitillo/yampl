#ifndef YAMPL_SHM_SIMPLESERVERSOCKET_H
#define YAMPL_SHM_SIMPLESERVERSOCKET_H

#include "yampl/generic/SimpleServerSocket.h"
#include "yampl/shm/ProducerSocket.h"
#include "yampl/shm/ConsumerSocket.h"

namespace yampl{
namespace shm{

typedef SimpleServerSocket<ProducerSocket, ConsumerSocket> SimpleServerSocket;

}
}

#endif
