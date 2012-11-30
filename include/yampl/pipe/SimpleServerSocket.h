#ifndef YAMPL_PIPE_SIMPLESERVERSOCKET_H
#define YAMPL_PIPE_SIMPLESERVERSOCKET_H

#include "yampl/generic/SimpleServerSocket.h"
#include "yampl/pipe/ProducerSocket.h"
#include "yampl/pipe/ConsumerSocket.h"

namespace yampl{
namespace pipe{

typedef SimpleServerSocket<ProducerSocket, ConsumerSocket> SimpleServerSocket;

}
}

#endif
