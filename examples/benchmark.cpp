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

#include "ZMQ/SocketFactory.h"
#include "pipe/SocketFactory.h"

using namespace IPC;
using namespace std;

static struct timeval start, end;
static long seconds, useconds;

inline void deallocator(void *, void*){}

void start_clock(){
  gettimeofday(&start, NULL);
}

long stop_clock(){
  gettimeofday(&end, NULL);
  seconds = end.tv_sec - start.tv_sec;
  useconds = end.tv_usec - start.tv_usec;
  return ((seconds) * 1000000 + useconds) + 0.5;
}

ISocketFactory *createFactory(const char *impl){
  if(strcasecmp(impl, "splice") == 0){
    cout << "using SpliceSocket" << endl;
    return new pipe::SocketFactory(true);
  }else if(strcasecmp(impl, "pipe") == 0){
    cout << "using PipeSocket" << endl;
    return new pipe::SocketFactory(false);
  }else{
    cout << "using ZMQSocket" << endl;
    return new ZMQ::SocketFactory();
  }
}

Topology parseTopology(const char *t){
  if(strcasecmp(t, "MANY_TO_MANY") == 0)
    return MANY_TO_MANY;
  else if(strcasecmp(t, "MANY_TO_ONE") == 0)
    return MANY_TO_ONE;
  else if(strcasecmp(t, "ONE_TO_MANY") == 0)
    return ONE_TO_MANY;
  else
    return ONE_TO_ONE;
}

int main(int argc, char *argv[]){
  int opt;
  const char *impl = "zmq";
  unsigned iterations = 1000;
  unsigned size = 1000000;
  const char *s_buffer = 0;
  char *r_buffer = 0;
  Topology topology = ONE_TO_ONE;

  while((opt = getopt(argc, argv, "i:n:s:t:")) != -1){
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
	topology = parseTopology(optarg);
	break;
      default:
	fprintf(stderr, "Usage: %s [-i impl] [-t topology] [-n iterations] [-s size]\n", argv[0]);
	exit(EXIT_FAILURE);
    }
  }

  Channel channel("service", topology);

  if(fork() == 0){
    cout << "Creating Client ";

    s_buffer = new char[size];
    ISocketFactory *factory = createFactory(impl);
    ISocket *socket = factory->createClientSocket(channel, true, deallocator);

    for(size_t i = 0; i < iterations; i++){
      socket->send(s_buffer, size);
      socket->recv(&r_buffer);
    }

    delete socket;
    delete factory;
  }else{
    cout << "Creating Server ";

    s_buffer = new char[size];
    ISocketFactory *factory = createFactory(impl);
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
    delete factory;
  }

  return 0;
}
