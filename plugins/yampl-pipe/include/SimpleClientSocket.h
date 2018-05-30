#ifndef YAMPL_PIPE_SIMPLECLIENTSOCKET_H
#define YAMPL_PIPE_SIMPLECLIENTSOCKET_H

#include "yampl/generic/SimpleClientSocket.h"
#include "ProducerSocket.h"
#include "ConsumerSocket.h"

namespace yampl{
namespace pipe{

typedef SimpleClientSocket<ProducerSocket, ConsumerSocket> SimpleClientSocket;

}
}

#endif
