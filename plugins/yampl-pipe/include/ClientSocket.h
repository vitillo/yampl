#ifndef YAMPL_PIPE_CLIENTSOCKET_H
#define YAMPL_PIPE_CLIENTSOCKET_H

#include "yampl/generic/ClientSocket.h"
#include "SimpleClientSocket.h"

namespace yampl {
    namespace pipe {

        typedef ClientSocket<SimpleClientSocket> ClientSocket;

    }
}

#endif
