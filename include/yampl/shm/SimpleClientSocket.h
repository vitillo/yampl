#ifndef YAMPL_SHM_SIMPLECLIENTSOCKET_H
#define YAMPL_SHM_SIMPLECLIENTSOCKET_H

#include "yampl/generic/SimpleClientSocket.h"
#include "yampl/shm/ProducerSocket.h"
#include "yampl/shm/ConsumerSocket.h"

namespace yampl{
namespace shm{

typedef SimpleClientSocket<ProducerSocket, ConsumerSocket> SimpleClientSocket;

}
}

#endif
