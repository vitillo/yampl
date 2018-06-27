#ifndef YAMPL_SHM_SIMPLESERVERSOCKET_H
#define YAMPL_SHM_SIMPLESERVERSOCKET_H

#include "yampl/generic/SimpleServerSocket.h"
#include "ProducerSocket.h"
#include "ConsumerSocket.h"

namespace yampl{
namespace shm{

typedef SimpleServerSocket<ProducerSocket, ConsumerSocket> SimpleServerSocket;

}
}

#endif
