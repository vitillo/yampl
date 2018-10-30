#ifndef YAMPL_SHM_SIMPLECLIENTSOCKET_H
#define YAMPL_SHM_SIMPLECLIENTSOCKET_H

#include "yampl/generic/SimpleClientSocket.h"
#include "ProducerSocket.h"
#include "ConsumerSocket.h"

namespace yampl{
namespace shm{

typedef SimpleClientSocket<ProducerSocket, ConsumerSocket> SimpleClientSocket;

}
}

#endif
