#include <unistd.h>
#include <getopt.h>
#include <sys/time.h>

#include <iostream>

#include "ZMQSocketFactory.h"
#include "PipeSocketFactory.h"

using namespace IPC;
using namespace std;

inline void deallocator(void *, void*){}
static struct timeval start, end;
static long seconds, useconds;

void start_clock(){
  gettimeofday(&start, NULL);
}

long stop_clock(){
  gettimeofday(&end, NULL);
  seconds = end.tv_sec - start.tv_sec;
  useconds = end.tv_usec - start.tv_usec;
  return ((seconds) * 1000 + useconds/1000.0) + 0.5;
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
  Channel channel("pipe", ONE_TO_ONE);

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

  if(fork() > 0){
    cout << "Creating Sender" << endl;
    const char *buffer = new char[size];
    ISocketFactory *factory = createFactory(impl);
    ISocket *socket = factory->createProducerSocket(channel, true, deallocator);
    
    for(size_t i = 0; i < iterations; i++){
      socket->send(buffer, size);
    }

    cout << iterations << " messages of " << size << " bytes sent." << endl;

    char *a = new char[size];
    char *b = new char[size];

    start_clock();
    for(size_t i = 0; i < iterations; i++)
      memcpy(a, b, size);
    
    cout << iterations << " memory copies of " << size << " bytes performed in " << stop_clock() << " milliseconds" << endl;
  }else{
    cout << "Creating Receiver" << endl;
    char *buffer = 0;
    ISocketFactory *factory = createFactory(impl);
    ISocket *socket = factory->createConsumerSocket(channel);

    start_clock();
    for(size_t i = 0; i < iterations; i++){
      socket->receive((void **)&buffer);
    }

    cout << iterations << " messages of " << size << " bytes received in " << stop_clock() << " milliseconds" << endl;
  }

  wait();
  return 0;
}
