#ifndef YAMPL_PIPE_SIMPLESERVERSOCKET_H
#define YAMPL_PIPE_SIMPLESERVERSOCKET_H

#include "yampl/generic/SimpleServerSocket.h"
#include "ProducerSocket.h"
#include "ConsumerSocket.h"

namespace yampl{
namespace pipe{

typedef SimpleServerSocket<ProducerSocket, ConsumerSocket> SimpleServerSocket;

}
}

#endif
