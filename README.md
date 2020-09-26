# YAMPL

## Introduction

YAMPL (Yet Another Message Passing Library) is a C++ library that provides a simple abstraction of inter-process (local or distributed) & inter-thread communication channels.

A channel allows to send and receive data over it. Each end of a channel is attached to a socket:

* **ClientSocket:**  a *ClientSocket* can be connected to at most one *ServerSocket* through a channel;
* **ServerSocket:** a *ServerSocket* can be connected to zero ore more *ClientSocket*-s through a channel;

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

### Modular architecture

Each of the strategies listed anbove is implemented as a modular and independent plugin. The plugin subsystem (`PluginArbiter`) allows for an easy integration of new custom plugins. When YAMPL is executed, the `PluginArbiter` loads all available plugins at runtime, handles memory management and all the bookkeeping. The `plugin` subdirectory currently contains the code of three plugins. Each of them implements a specific backend for YAMPL:

* **yampl-shm:** Shared Memory backend;
* **yampl-pipe:** Linux Pipe backend;
* **yampl-zmq:** [ZeroMQ](https://zeromq.org) backend;

The users can easily develop their own plugins for YAMPL (e.g. for implementing IPC backends different from those listed above). YAMPL uses a strict API defining how both plugin and application must behave in order to guarantee a correct operation. [This](https://github.com/ntauth/yampl-example) is a repository containing documentation and a minimal example of how to implement a plugin for YAMPL.

## Build, Test & Install
``` bash
git clone https://github.com/vitillo/yampl
cd yampl
# (Optional) Pull additional plugins. See the documentation at https://github.com/ntauth/yampl-example
# ...
# Configure project
mkdir /path/to/build/directory
cd /path/to/build/directory
cmake /path/to/repo/root -DCMAKE_INSTALL_PREFIX=/path/to/install/dir
# Build and install
make
make install
# Run tests
make test
# Run benchmarks
make example
```

A number of flags can be passed to CMake to customize the build:

- `WITH_EXAMPLES`: Include examples in the build. **Default: ON**
- `WITH_TESTS`: Include tests in the build. **Default: ON**
- `WITH_PLUGIN_[PluginId]`: Include plugin yampl-[PluginId] in the build. **Default: ON for SHM, PIPE and ZMQ**
- `WITH_PYBIND`: Include Python bindings in the build. **Default: ON**
- `PYBIND_ONLY`: Build Python bindings only. **Default: OFF**
- `PYTHON_EXECUTABLE:FILEPATH`: Path to the Python binary. **Default: Automatically Detected**
- `PYTHON_INCLUDE_DIR:PATH`: Path to the Python include directory. **Default: Automatically Detected**
- `PYTHON_LIBRARY:FILEPATH`: Path to the Python library. **Default: Automatically Detected**

**Note** that if `WITH_PYBIND` is enabled, a working version of Python >= 2.7 with the `setuptools` library installed is required. If you don't have `setuptools`, you can install it using ```pip install setuptools```.
If you have multiple versions of Python installed in the system, you can use the `PYTHON_*` flags listed above to point to a specific version of Python.

To compile and link against the YAMPL library, assuming `${CMAKE_INSTALL_PREFIX}/lib/pkgconfig/` is in `PKG_CONFIG_PATH`:
```bash
g++ `pkg-config yampl --libs --cflags` foo.cpp
```

YAMPL is by default installed in `$HOME/yampl` unless you provide a `CMAKE_INSTALL_PREFIX` when configuring the project with CMake. Another file, `.yamplrc` is installed in `$HOME` and is used by the plugin manager to locate the plugins installed in the system.

**NOTE**: Depending on your system, using pipes as the communication strategy may require elevated privileges. This depends on the maximum pipe buffer size reported in `/proc/sys/fs/pipe-max-size`: YAMPL sets the buffer size to 1048576 bytes to maximize performance but, if this exceeds `pipe-max-size`, the consumer process will need to be run with elevated privileges or with `CAP_SYS_RESOURCE` on. If your work environment doesn't allow this, performance will be limited, so another communication strategy would be the better fit.

## Python bindings
YAMPL is automatically built with `-DWITH_PYBIND=ON`, which seamlessly generates Python bindings. The bindings are installed in `${CMAKE_INSTALL_PREFIX}/python`. In order to import the bindings, the
`PYTHONPATH` environment variable pointing to `${CMAKE_INSTALL_PREFIX}/python` has to be exported. After that the bindings can be used within Python as follows:

```python
import yampl

socket = ClientSocket('<server_name>', '<socket_type>')
socket.send({ 'hello world!' })
```

For complete implementation of a Hello World example for python Yampl see:

* `tests/server_test.py	`
* `tests/client_test.py`

