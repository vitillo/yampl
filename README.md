# YAMPL

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

## Build & Install
``` bash
git clone https://github.com/vitillo/yampl
cd yampl
# Configure project
mkdir /path/to/build/directory
cd /path/to/build/directory
cmake /path/to/repo/root -DCMAKE_INSTALL_PREFIX=/path/to/install/dir
# Build and install
make
make install
```

YAMPL is by default installed in `$HOME/yampl` unless you provide a `CMAKE_INSTALL_PREFIX` when configuring the project with CMake.
