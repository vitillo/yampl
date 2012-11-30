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
#include <vector>

#include "yampl/ZMQ/SocketFactory.h"
#include "yampl/pipe/SocketFactory.h"
#include "yampl/shm/SocketFactory.h"
#include "yampl/utils/Thread.h"

using namespace yampl;
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

ISocketFactory *parseFactory(const char *impl){
  if(strcasecmp(impl, "pipe") == 0){
    return new pipe::SocketFactory();
  }else if(strcasecmp(impl, "shm") == 0){
    return new shm::SocketFactory();
  }else{
    return new ZMQ::SocketFactory();
  }
}

Context parseContext(const char *c, string &channelName){
  if(strcasecmp(c, "LOCAL") == 0){
    cout << "context: LOCAL" << endl;
    return LOCAL;
  }else if(strcasecmp(c, "THREAD") == 0){
    cout << "context: THREAD" << endl;
    return THREAD;
  }else if(strcasecmp(c, "DISTRIBUTED") == 0){
    cout << "context: DISTRIBUTED" << endl;
    channelName = "127.0.0.1:3333";
    return DISTRIBUTED;
  }

  exit(-1);
}

void dumpImpl(const char *impl){
  if(strcasecmp(impl, "pipe") == 0){
    cout << "implementation: PipeSocket" << endl;
  }else if(strcasecmp(impl, "shm") == 0){
    cout << "implementation: SHMSocket" << endl;
  }else{
    cout << "implementation: ZMQSocket" << endl;
  }
}

void dumpSemantics(Semantics semantics){
  if(semantics == MOVE_DATA){
    cout << "semantics: MOVE_DATA" << endl;
  }else{
    cout << "semantics: COPY_DATA" << endl;
  }
}

void client(ISocketFactory *factory, Channel channel, Semantics semantics, void *s_buffer, void *r_buffer, unsigned size, unsigned iterations){
  s_buffer = new char[size];
  ISocket *socket = factory->createClientSocket(channel, semantics, deallocator);

  for(size_t i = 0; i < iterations; i++){
    socket->send(s_buffer, size);
    socket->recv(r_buffer, size);
  }

  delete socket;
}

void server(ISocketFactory *factory, Channel channel, Semantics semantics, void *s_buffer, void *r_buffer, unsigned size, unsigned iterations, unsigned multiplicity){
  s_buffer = new char[size];
  ISocket *socket = factory->createServerSocket(channel, semantics, deallocator);

  start_clock();
  for(size_t i = 0; i < iterations * multiplicity; i++){
    socket->recv(r_buffer, size);
#ifndef NDEBUG
    assert(memcmp(s_buffer, r_buffer, size) == 0);
    memset((void *)r_buffer, 0, size);
#endif
    socket->send(s_buffer, size);
  }

  long t = stop_clock();
  cout << "Latency " << t / (2*iterations*multiplicity) << " microseconds"<< endl;
  cout << "Bandwidth " << (size*iterations*multiplicity*2)/t<< " MB/s" << endl;

  for(unsigned i = 0; i < multiplicity; i++){
    int status;
    wait(&status);
  }

  delete socket;
}

int main(int argc, char *argv[]){
  int opt;
  const char *impl = "zmq";
  const char *cont = "LOCAL";
  unsigned iterations = 1000;
  unsigned size = 1000000;
  unsigned multiplicity = 1;
  char *s_buffer = 0;
  char *r_buffer = 0;
  string channelName = "service";
  Context context = LOCAL;
  Semantics semantics = MOVE_DATA;

  while((opt = getopt(argc, argv, "i:n:s:yc:m:")) != -1){
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
      case 'y':
	semantics = COPY_DATA;
	break;
      case 'c':
	cont = strdup(optarg);
	break;
      case 'm':
	multiplicity = atoi(optarg);
	break;
      default:
	fprintf(stderr, "Usage: %s [-i impl] [-y] [-c context] [-n iterations] [-s size]\n", argv[0]);
	exit(EXIT_FAILURE);
    }
  }

  cout << "iterations: " << iterations << endl;
  cout << "buffer size: " << size << endl;
  cout << "multiplicity: " << multiplicity << endl;

  dumpImpl(impl);
  context = parseContext(cont, channelName);
  dumpSemantics(semantics);
  Channel channel(channelName, context);
  s_buffer = new char[size];
  r_buffer = new char[size];

  if(channel.context == LOCAL|| channel.context == DISTRIBUTED){
    for(unsigned i = 0; i < multiplicity; i++){
      if(fork() == 0){
	ISocketFactory *factory = parseFactory(impl);
	client(factory, channel, semantics, s_buffer, r_buffer, size, iterations);
	return 0;
      }else{
	continue;
      }
    }

    ISocketFactory *factory = parseFactory(impl);
    server(factory, channel, semantics, s_buffer, r_buffer, size, iterations, multiplicity);
  }else if(channel.context == THREAD){
    ISocketFactory *factory = parseFactory(impl);
    vector<Thread*> threads;

    Thread s(tr1::bind(server, factory, channel, semantics, s_buffer, r_buffer, size, iterations, multiplicity));

    for(unsigned i = 0; i < multiplicity; i++){
      threads.push_back(new Thread(tr1::bind(client, factory, channel, semantics, s_buffer, r_buffer, size, iterations)));
    }

    for(unsigned i = 0; i < multiplicity; i++){
      threads[i]->join();
    }

    s.join();
  }

  return 0;
}
