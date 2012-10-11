#include <unistd.h>
#include <getopt.h>
#include <sys/time.h>
#include <sys/wait.h>

#include <iostream>
#include <cstring>
#include <cassert>
#include <cstdlib>

#include "ZMQSocketFactory.h"
#include "PipeSocketFactory.h"

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
    return new PipeSocketFactory(true);
  }else if(strcasecmp(impl, "pipe") == 0){
    cout << "using PipeSocket" << endl;
    return new PipeSocketFactory(false);
  }else{
    cout << "using ZMQSocket" << endl;
    return new ZMQSocketFactory();
  }
}

int main(int argc, char *argv[]){
  int opt;
  const char *impl = "zmq";
  unsigned iterations = 1024;
  unsigned size = 1048576;
  const char *s_buffer = 0;
  const char *r_buffer = 0;
  Channel channel("service", ONE_TO_ONE);

  while((opt = getopt(argc, argv, "i:n:s:")) != -1){
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
      default:
	fprintf(stderr, "Usage: %s [-i impl] [-n iterations] [-s size]\n", argv[0]);
	exit(EXIT_FAILURE);
    }
  }

  s_buffer = new char[size];

  if(fork() == 0){
    cout << "Creating Client ";
    ISocketFactory *factory = createFactory(impl);
    ISocket *socket = factory->createClientSocket(channel, true, deallocator);

    for(size_t i = 0; i < iterations; i++){
      socket->send(s_buffer, size);
      socket->receive((void **)&r_buffer);
    }

    delete socket;
    delete factory;
  }else{
    cout << "Creating Server ";
    ISocketFactory *factory = createFactory(impl);
    ISocket *socket = factory->createServerSocket(channel, true, deallocator);
    
    start_clock();
    for(size_t i = 0; i < iterations; i++){
      socket->receive((void **)&r_buffer);
      socket->send(s_buffer, size);
    }

    long t = stop_clock();
    cout << "Latency " << t / (2*iterations) << " microseconds"<< endl;
    cout << "Bandwidth " << (size * iterations * 2 )/t << "MB/s" << endl;

    int status;
    wait(&status);

    delete socket;
    delete factory;
  }

  return 0;
}
