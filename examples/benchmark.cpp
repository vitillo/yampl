#include <unistd.h>
#include <getopt.h>
#include <sys/time.h>
#include <sys/wait.h>

#include <cstring>
#include <iostream>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <thread>

#include "ZMQ/SocketFactory.h"
#include "pipe/SocketFactory.h"

using namespace IPC;
using namespace std;

static struct timeval start_time, end_time;
static long seconds, useconds;

inline void deallocator(void *, void*){}

void start_clock(){
  gettimeofday(&start_time, NULL);
}

long stop_clock(){
  gettimeofday(&end_time, NULL);
  seconds = end_time.tv_sec - start_time.tv_sec;
  useconds = end_time.tv_usec - start_time.tv_usec;
  return ((seconds) * 1000000 + useconds) + 0.5;
}

ISocketFactory *createFactory(const char *impl){
  if(strcasecmp(impl, "splice") == 0){
    cout << "implementation: SpliceSocket" << endl;
    return new pipe::SocketFactory(true);
  }else if(strcasecmp(impl, "pipe") == 0){
    cout << "implementation: PipeSocket" << endl;
    return new pipe::SocketFactory(false);
  }else{
    cout << "implementation: ZMQSocket" << endl;
    return new ZMQ::SocketFactory();
  }
}

Topology parseTopology(const char *t){
  if(strcasecmp(t, "MANY_TO_MANY") == 0){
    cout << "topology: MANY_TO_MANY" << endl;
    return MANY_TO_MANY;
  }else if(strcasecmp(t, "MANY_TO_ONE") == 0){
    cout << "topology: MANY_TO_ONE" << endl;
    return MANY_TO_ONE;
  }else if(strcasecmp(t, "ONE_TO_MANY") == 0){
    cout << "topology: ONE_TO_MANY" << endl;
    return ONE_TO_MANY;
  }else{
    cout << "topology: ONE_TO_ONE" << endl;
    return ONE_TO_ONE;
  }
}

Context parseContext(const char *c){
  if(strcasecmp(c, "LOCAL_PROCESS") == 0){
    cout << "context: LOCAL_PROCESS" << endl;
    return LOCAL_PROCESS;
  }else if(strcasecmp(c, "THREAD") == 0){
    cout << "context: THREAD" << endl;
    return THREAD;
  }else{
    cout << "context: DISTRIBUTED_PROCESS" << endl;
    return DISTRIBUTED_PROCESS;
  }
}

void client(ISocketFactory *factory, Channel channel, void *s_buffer, void *r_buffer, unsigned size, unsigned iterations){
  cout << "Creating Client " << endl;

  s_buffer = new char[size];
  ISocket *socket = factory->createClientSocket(channel, true, deallocator);

  for(size_t i = 0; i < iterations; i++){
    socket->send(s_buffer, size);
    socket->recv(&r_buffer);
  }

  delete socket;
}

void server(ISocketFactory *factory, Channel channel, void *s_buffer, void *r_buffer, unsigned size, unsigned iterations){
  cout << "Creating Server " << endl;

  s_buffer = new char[size];
  ISocket *socket = factory->createServerSocket(channel, true, deallocator);
  
  start_clock();
  for(size_t i = 0; i < iterations; i++){
    socket->recv(&r_buffer);
#ifndef NDEBUG
    assert(memcmp(s_buffer, r_buffer, size) == 0);
    memset((void *)r_buffer, 0, size);
#endif
    socket->send(s_buffer, size);
  }

  long t = stop_clock();
  cout << "Latency " << t / (2*iterations) << " microseconds"<< endl;
  cout << "Bandwidth " << (size * iterations * 2)/t<< " MB/s" << endl;

  int status;
  wait(&status);

  delete socket;
}

int main(int argc, char *argv[]){
  int opt;
  const char *impl = "zmq";
  const char *topo = "ONE_TO_ONE";
  const char *cont = "LOCAL_PROCESS";
  unsigned iterations = 1000;
  unsigned size = 1000000;
  char *s_buffer = 0;
  char *r_buffer = 0;
  Topology topology = ONE_TO_ONE;
  Context context = LOCAL_PROCESS;

  while((opt = getopt(argc, argv, "i:n:s:t:c:")) != -1){
    switch(opt){
      case 'i':
	impl = strdup(optarg);
	break;
      case 'n':
	iterations = atoi(optarg);
	break;
      case 's':
	size = atoi(optarg);
	break;
      case 't':
	topo = strdup(optarg);
	break;
      case 'c':
	cont = strdup(optarg);
	break;
      default:
	fprintf(stderr, "Usage: %s [-i impl] [-t topology] [-n iterations] [-s size]\n", argv[0]);
	exit(EXIT_FAILURE);
    }
  }

  cout << "iterations: " << iterations << endl;
  cout << "buffer size: " << size << endl;

  topology = parseTopology(topo);
  context = parseContext(cont);
  Channel channel("service", topology, context);

  if(channel.context == LOCAL_PROCESS){
    if(fork() == 0){
      ISocketFactory *factory = createFactory(impl);
      client(factory, channel, s_buffer, r_buffer, size, iterations);
    }else{
      ISocketFactory *factory = createFactory(impl);
      server(factory, channel, s_buffer, r_buffer, size, iterations);
    }
  }else if(channel.context == THREAD){
    ISocketFactory *factory = createFactory(impl);

    thread c([&]{
      client(factory, channel, s_buffer, r_buffer, size, iterations);
    });

    thread s([&]{
      server(factory, channel, s_buffer, r_buffer, size, iterations);
    });

    c.join();
    s.join();
  }

  return 0;
}
