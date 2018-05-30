# YAMPL Core

[![Build Status](https://travis-ci.org/ntauth/yampl.svg?branch=master)](https://travis-ci.org/ntauth/yampl)

YAMPL (Yet Another Message Passing Library) provides a simple abstraction of inter-process (local or distributed) & inter-thread communication channels.

A channel allows to send and receive data over it. Each end of a channel is attached to a socket:
* **ClientSocket:**  a ***ClientSocket*** can be connected to at most a single ***ServerSocket*** through a channel;
* **ServerSocket:** a ***ServerSocket*** can be connected to zero ore more ***ClientSocket*** through a channel;

YAMPL allows to send and receive: 
* objects of trivially copiable types
* arrays of trivially copiable types

A **trivially copiable** type
* has no non-trivial copy constructors (no virtual functions or virtual bases)
* has no non-trivial copy assignment constructors
* has a trivial destructor

In other words, a trivially copyable type is any type for which the underlying bytes can be copied to an array of char or unsigned char and into a new object of the same type, and the resulting object would have the same value as the original.

Different communication strategies are offered to provide the best performances for the given problem:
* **Inter-thread:** Lock Free Queues
* **Inter-process (local):**
    * "small" messages: Lock Free Queues over POSIX Shared Memory, optimized for latency
    * "big" messages: UNIX Pipes (vmsplice), optimized for bandwidth
* **Inter-process (distributed):** POSIX Sockets 

Each and all of these strategies are implemented as modular and independent plugins that can easily be pulled from Github, built and installed in the system. The plugin subsystem (***PluginArbiter***) allows for an easy integration of new custom plugins. When YAMPL is executed, the ***PluginArbiter*** loads all the plugins at runtime, handles memory management and all the bookkeeping. These are the currently available plugins:
* **yampl-shm:** SHM backend for YAMPL - [repository](https://github.com/ntauth/yampl-shm)
* **yampl-pipe:** Pipe backend for YAMPL - [repository](https://github.com/ntauth/yampl-pipe)
* **yampl-zmq:** ZeroMQ backend for YAMPL - [repository](https://github.com/ntauth/yampl-zmq)

## Build, Test & Install
``` bash
git clone --recurse-submodules https://github.com/ntauth/yampl
cd yampl
# Pull additional plugins
cd plugins
git clone https://github.com/{user_name}/{plugin_name}
# Configure project
mkdir /path/to/build/directory
cd /path/to/build/directory
cmake /path/to/repo/root -DWITH_EXAMPLES={ON(DEFAULT)|OFF} -DWITH_TESTS={ON(DEFAULT)|OFF} -DWITH_PLUGIN_{SHM|ZMQ|PIPE}={ON(Default)|OFF} -DCMAKE_INSTALL_PREFIX=/path/to/install/directory
# Build and install
make
make install
```

To compile and link against the YAMPL library:
```
g++ `pkg-config yampl --libs --cflags` foo.cpp
```

## Examples
The *examples* subdirectory provides binaries that demonstrate some use-cases.

### Local Client
The clients ping a server process and receive a reply from it.

``` c++
#include <unistd.h>
#include <iostream>

#include "yampl.h"

using namespace std;
using namespace yampl;

int main(int argc, char *argv[]) {
  char pong[100];
  const string ping = "ping from " + to_string(getpid());
  
  Channel channel("service", LOCAL_SHM);
  ISocketFactory* factory = new SocketFactory();
  ISocket* socket = factory->createClientSocket(channel);

  while(true) {
    socket->send(ping);
    socket->recv(pong);
    cout << pong << endl;
  }
}
```

### Local Server
The server process replies to the pings of the client.

```c++
#include <unistd.h>
#include <iostream>

#include "yampl.h"

using namespace std;
using namespace yampl;

int main(int argc, char *argv[]) {
  char ping[100];

  Channel channel("service", LOCAL_SHM);
  ISocketFactory* factory = new SocketFactory();
  ISocket* socket = factory->createServerSocket(channel);

  while(true) {
    socket->recv(ping);
    socket->send("pong");
    cout << ping << endl;
    sleep(1);
  }
}
```

### Distributed Producer
```c++
#include <unistd.h>
#include <iostream>

#include "yampl.h"

using namespace yampl;
using namespace std;

void deallocator(void *, void*) {}

int main(int argc, char *argv[]) {
  string message = "Hello from " +  to_string(getpid());
  
  Channel channel("127.0.0.1:3333", DISTRIBUTED);
  ISocketFactory* factory = new SocketFactory();
  ISocket* socket = factory->createClientSocket(channel, MOVE_DATA, deallocator);

  while(true) {
    socket->send(message);
    cout << "Message sent" <<  endl;
    sleep(1);
  }
}
```

### Distributed Consumer
```c++
#include <iostream>

#include "yampl.h"

using namespace std;
using namespace yampl;

int main(int argc, char *argv[]) {
  char* message = 0;

  Channel channel("127.0.0.1:3333", DISTRIBUTED);
  ISocketFactory* factory = new SocketFactory();
  ISocket* socket = factory->createServerSocket(channel, MOVE_DATA);

  while(true) {
    socket->recv(message);
    cout << message << endl;
  }
}
```

### Multithreaded Client-Server
The following is a similar example to the above one but this time in a multithreaded environment (C++11).


```c++
#include <unistd.h>
#include <iostream>
#include <thread>

#include "yampl.h"

inline void deallocator(void *, void*) {}

using namespace std;
using namespace yampl;

int main(int argc, char *argv[])
{
  const int nThreads = 10;
  ISocketFactory* factory = new SocketFactory();

  thread server([factory] {
    Channel channel("service", THREAD);
    ISocket *socket = factory->createServerSocket(channel, MOVE_DATA, deallocator);

    while(true) {
      cout << "Ping from client " << socket->recv<int>() << endl;
      socket->send(0);
      sleep(1);
    }
  });
  
  for(int i = 0; i < nThreads; i++) {
    thread t([factory, i] {
      Channel channel("service", THREAD);
      ISocket* socket = factory->createClientSocket(channel, MOVE_DATA, deallocator);

      while(true){
        socket->send(i);
        socket->recv<int>();
      }
    });

    t.detach();
  }

  server.join();
}
```
