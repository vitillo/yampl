# ATLAS IPC library

The library provides a simple abstraction of inter-process communication channels for specific patterns.The supported patterns are:
* **Producer - Consumer:** A producer sends data to a receiver.
* **Client - Server:** A client sends a request to a server and receives a reply back.

A channel allows to send and receive data on it. Receives are blocking while sends are buffered and block only when the buffer is filled up. Each end of a channel is attached to a socket, i.e. for the **Client - Server** pattern there is one or more **ClientSockets** and one or more **ServerSockets**.

 A channel has a certain topology, i.e. for the case of the **Producer - Consumer** pattern:
* **One to One:** one producer and one consumer
* **One to Many:** one producer and many consumer; messages are handed over with a roundrobin policy
* **Many to One:** many producers and one consumer
* **Many to Many:** many producers and many consumers

## Build
Run make to build the library and the examples.

##Examples
The *examples* subdirectory provides four binaries that demonstrate the two supported patterns:
* *client* and *server* instantiate the **Client - Server** pattern with a **Many to One** channel, that means you can launch multiple client and one server
* *producer* and *consumer* instantiate the **Producer - Consumer** pattern with a **One to Many** channel, that means there can be only one producer but multiple consumers

