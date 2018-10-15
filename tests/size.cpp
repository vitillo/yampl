#include <unistd.h>
#include <cassert>
#include <iostream>
#include <sys/wait.h>

#include "yampl.h"

using namespace std;
using namespace yampl;

const char message[] = "Hello World!";

void client(ISocketFactory* factory, const Channel &channel)
{
    char *buffer = new char[100];
    ISocket *socket = factory->createClientSocket(channel);

    socket->send(message);

    try {
        socket->recv(buffer, 1);
    } catch (InvalidSizeException) {
        socket->recv(buffer, 100);
    }

    assert(strcmp(buffer, message) == 0);

    delete[] buffer;
    delete socket;
}

void server(ISocketFactory* factory, const Channel &channel)
{
    char* buffer = new char[100];
    ISocket* socket = factory->createServerSocket(channel);

    try {
        socket->recv(buffer, 1);
    } catch (InvalidSizeException) {
        socket->recv(buffer, 100);
    }

    assert(strcmp(buffer, message) == 0);
    socket->send(message);

    delete[] buffer;
    delete socket;
}

int main(int argc, char *argv[])
{
    int status = -1;
    ISocketFactory* factory;

    if(fork() == 0)
    {
        factory = new SocketFactory();

        client(factory, Channel("zmq", LOCAL));
        client(factory, Channel("pipe", LOCAL_PIPE));
        client(factory, Channel("shm", LOCAL_SHM));

        delete factory;
    }
    else
    {
        factory = new SocketFactory();

        server(factory, Channel("zmq", LOCAL));
        server(factory, Channel("pipe", LOCAL_PIPE));
        server(factory, Channel("shm", LOCAL_SHM));

        wait(&status);
        cout << "[size] Success" << endl;
        status = 0;

        delete factory;
    }

    return status;
}
