# YAMPL

YAMPL (Yet Another Message Passing Library) provides a simple abstraction of inter-process (local or distributed) & inter-thread communication channels.

A channel allows to send and receive data over it. Receives are blocking while sends are buffered and block only when the internal buffer is full. Each end of a channel is attached to a socket:
* **ClientSocket:**  a ***ClientSocket*** can be connected to at most a single ***ServerSocket*** through a channel;
* **ServerSocket:** a ***ServerSocket*** can be connected to zero ore more ***ClientSocket***s through a channel;

The implementation determines at run-time the best communication strategy possible in order to reduce the latency and to increase the bandwidth for the current communication pattern:
* **Inter-thread:** Lock Free Queues
* **Inter-process (local)**
    * **"small" messages:** Lock Free Queues over POSIX Shared Memory 
    * **"big" messages:** UNIX Pipes (vmsplice)
* **Inter-process (distributed):** POSIX Sockets 

## Build, Test & Install
``` bash
git clone https://github.com/vitillo/yampl
cd yampl
./configure --prefix=INSTALL_PATH
make
make check
make install
```
To compile and link against the YAMPL library:
```
g++ `pkg-config yampl --libs --cflags` foo.c
```

##Examples
The *examples* subdirectory provides binaries that demonstrate some use-cases.

###Local Client
The clients ping a server process and receive a reply from it.

``` c++
#include <unistd.h>
#include <iostream>

#include "yampl.h"

using namespace yampl;
using namespace std;

int main(int argc, char *argv[]){
  const string ping = "Ping from " + to_string(getpid());
  char pong[100], *pong_ptr = &pong[0];
  
  Channel channel("service");
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createClientSocket(channel);

  while(true){
    socket->send(ping.c_str(), ping.size() + 1);
    socket->recv(&pong_ptr, sizeof(pong));
    cout << pong << endl;
  }
}
```

###Local Server
The server process replies to the pings of the client.

```c++
#include <unistd.h>
#include <iostream>

#include "yampl.h"

using namespace std;
using namespace yampl;

int main(int argc, char *argv[]){
  char ping[100], *ping_ptr = &ping[0];
  string pong = "Pong from " + to_string(getpid());
  
  Channel channel("service");
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

###Distributed Producer
```c++
#include <unistd.h>
#include <iostream>

#include "yampl.h"

using namespace yampl;
using namespace std;

void deallocator(void *, void*){}

int main(int argc, char *argv[]){
  string message = "Hello from " +  to_string(getpid());
  
  Channel channel("127.0.0.1:3333", DISTRIBUTED_PROCESS);
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createClientSocket(channel, MOVE_DATA, deallocator);

  while(true){
    socket->send(message.c_str(), message.size());
    cout << "Message sent" <<  endl;
    sleep(1);
  }
}
```

###Distributed Consumer
```c++
#include <iostream>

#include "yampl.h"

using namespace std;
using namespace yampl;

int main(int argc, char *argv[]){
  char *message = 0;

  Channel channel("127.0.0.1:3333", DISTRIBUTED_PROCESS);
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createServerSocket(channel, MOVE_DATA);

  while(true){
    socket->recv(&message);
    cout << message << endl;
  }
}
```

###Multithreaded Client-Server
The following is a similar example to the above one but this time in a multithreaded environment (C++11).


```c++
#include <unistd.h>
#include <iostream>
#include <thread>

#include "yampl.h"

inline void deallocator(void *, void*){}

using namespace std;
using namespace yampl;

int main(int argc, char *argv[]){
  const int nThreads = 10;
  ISocketFactory *factory = new SocketFactory();

  thread server([factory] {
    Channel channel("service", THREAD);
    ISocket *socket = factory->createServerSocket(channel, MOVE_DATA, deallocator);

    while(true){
      cout << "Ping from client " << socket->recv<int>() << endl;
      socket->send(0);
      sleep(1);
    }
  });
  
  for(int i = 0; i < nThreads; i++){
    thread t([factory, i] {
      Channel channel("service", THREAD);
      ISocket *socket = factory->createClientSocket(channel, MOVE_DATA, deallocator);

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
