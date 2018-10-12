#include <unistd.h>
#include <iostream>

#include "yampl.h"

using namespace std;
using namespace yampl;

int main(int argc, char *argv[])
{
    char ping[100];

    cout << "[+] Server launched!" << endl;
    Channel channel("service", LOCAL_SHM);
    ISocketFactory *factory = new SocketFactory();
    cout << "[+] Creating server socket..." << endl;
    ISocket *socket = factory->createServerSocket(channel);

    cout << "[+] Socket created!" << endl;
    while(true){
        socket->recv(ping);
        socket->send("pong");
        cout << ping << endl;
        sleep(1);
    }
}
