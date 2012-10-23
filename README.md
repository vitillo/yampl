# ATLAS IPC library

This Linux library provides a simple abstraction of inter-process communication channels for specific patterns. The supported patterns are:
* **Producer - Consumer:** A producer sends data to a receiver;
* **Client - Server:** A client sends a request to a server and receives a reply back;

A channel allows to send and receive data on it. Receives are blocking while sends are buffered and block only when the buffer is filled up. Each end of a channel is attached to a socket, i.e. for the **Client - Server** pattern there are one or more **ClientSocket**s and one or more **ServerSocket**s.

 A channel has a certain topology, i.e. in the case of the **Producer - Consumer** pattern the options are:
* **One to One:** One producer and one consumer;
* **One to Many:** One producer and many consumer; messages are handed over with a roundrobin policy;
* **Many to One:** Many producers and one consumer:
* **Many to Many:** Many producers and many consumers;

## Build, Test & Install
Even though the library requires a C++11 compatible compiler to build it, it is still possible to compile and link against it from a non C++03 compiler. 

* git clone https://github.com/vitillo/ipc
* cd ipc
* ./configure --prefix=INSTALL_PATH
* make
* make check
* make install

##Examples
The *examples* subdirectory provides four binaries that demonstrate the two supported patterns:

* *client* and *server* instantiates the **Client - Server** pattern with a **Many to One** channel, that means you can launch multiple clients with one server
* *producer* and *consumer* instantiates the **Producer - Consumer** pattern with a **One to Many** channel, that means there can be only one producer but multiple consumers

###Clients
The clients ping a server process and receive a reply from it.

``` c++
#include <unistd.h>
#include <iostream>
#include "SocketFactory.h"

using namespace IPC;
using namespace std;

int main(int argc, char *argv[]){
  const string ping = "Ping from " + to_string(getpid());
  char pong[100], *pong_ptr = &pong[0];
  
  Channel channel("service", MANY_TO_ONE);
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createClientSocket(channel);

  while(true){
    socket->send(ping.c_str(), ping.size() + 1);
    socket->recv(&pong_ptr, sizeof(pong));
    cout << pong << endl;
  }
}
```

###Server
The server process replies to the pings of the client.

```c++
#include <unistd.h>
#include <iostream>
#include "SocketFactory.h"

using namespace std;
using namespace IPC;

int main(int argc, char *argv[]){
  char ping[100], *ping_ptr = &ping[0];
  string pong = "Pong from " + to_string(getpid());
  
  Channel channel("service", MANY_TO_ONE);
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createServerSocket(channel);

  while(true){
    socket->recv(&ping_ptr, sizeof(ping));
    socket->send(pong.c_str(), pong.size() + 1);
    cout << ping << endl;
    sleep(1);
  }
}
```
