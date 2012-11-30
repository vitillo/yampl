#ifndef YAMPL_PIPE_SIMPLECLIENTSOCKET_H
#define YAMPL_PIPE_SIMPLECLIENTSOCKET_H

#include "yampl/generic/SimpleClientSocket.h"
#include "yampl/pipe/ProducerSocket.h"
#include "yampl/pipe/ConsumerSocket.h"

namespace yampl{
namespace pipe{

typedef SimpleClientSocket<ProducerSocket, ConsumerSocket> SimpleClientSocket;

}
}

#endif
