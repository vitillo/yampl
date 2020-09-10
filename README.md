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

Each and all of these strategies are implemented as modular and independent plugins that can easily be pulled from Github, built and installed in the system. The plugin subsystem (***PluginArbiter***) allows for an easy integration of new custom plugins. When YAMPL is executed, the ***PluginArbiter*** loads all the plugins at runtime, handles memory management and all the bookkeeping. These are the currently available plugins:
* **yampl-shm:** SHM backend for YAMPL - [repository](https://github.com/ntauth/yampl-shm)
* **yampl-pipe:** Pipe backend for YAMPL - [repository](https://github.com/ntauth/yampl-pipe)
* **yampl-zmq:** ZeroMQ backend for YAMPL - [repository](https://github.com/ntauth/yampl-zmq)

## Build, Test & Install
``` bash
git clone https://github.com/vitillo/yampl
cd yampl
# (Optional) Pull additional plugins
cd plugins
git clone https://github.com/vitillo/yampl-zmq
# Configure project
mkdir /path/to/build/directory
cd /path/to/build/directory
cmake /path/to/repo/root -DCMAKE_INSTALL_PREFIX=/path/to/install/dir
# Build and install
make
make install
# Run tests
make test
# Run examples
make example
```

A number of flags can be passed to CMake to customize the build:
- `WITH_EXAMPLES`: Include examples in the build. **Default: ON**
- `WITH_TESTS`: Include tests in the build. **Default: ON**
- `WITH_PLUGIN_[PluginId]`: Include plugin yampl-[PluginId] in the build. **Default: ON**

To compile and link against the YAMPL library, assuming `${CMAKE_INSTALL_PREFIX}/lib/pkgconfig/` is in `PKG_CONFIG_PATH`:
```bash
g++ `pkg-config yampl --libs --cflags` foo.cpp
```

YAMPL is by default installed in `$HOME/yampl` unless you provide a `CMAKE_INSTALL_PREFIX` when configuring the project with CMake. Another file, `.yamplrc` is installed in `$HOME` and is used by the plugin manager to locate the plugins installed in the system.

**NOTE**: Depending on your system, using pipes as the communication strategy may require elevated privileges. This depends on the maximum pipe buffer size reported in `/proc/sys/fs/pipe-max-size`: YAMPL sets the buffer size to 1048576 bytes to maximize performance but, if this exceeds `pipe-max-size`, the consumer process will need to be run with elevated privileges or with `CAP_SYS_RESOURCE` on. If your work environment doesn't allow this, performance will be limited, so another communication strategy would be the better fit.

## Examples
The *examples* subdirectory provides binaries that demonstrate some use-cases.

## How to create a plugin
YAMPL is now a modular library, which means anyone can make a plugin suiting their needs. YAMPL uses a strict API defining how both plugin and application must behave in order to guarantee a correct operation. [This](https://github.com/ntauth/yampl-example) is a repository containing documentation and a minimal example of how to implement a plugin for YAMPL.
