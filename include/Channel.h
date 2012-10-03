#ifndef IPC_CHANNEL_H
#define IPC_CHANNEL_H

#include <string>
#include <stdint.h>

#include "Exceptions.h"

namespace IPC{

const int INFINITY = -1;

enum Topology{
  ONE_TO_ONE,
  ONE_TO_MANY,
  MANY_TO_ONE,
  MANY_TO_MANY
};

class Environment{
public:
  Environment(): m_context(1){}

  zmq::context_t& getContext(){
    return m_context;
  }

private:
  zmq::context_t m_context;
};

struct Channel{
    Channel(Environment &env, const std::string &name, Topology topology = ONE_TO_ONE, uint64_t asynchronicity = 1) : environment(env), name(std::string("ipc://") + name), topology(topology), asynchronicity(asynchronicity){
      if(!asynchronicity)
	throw UnsupportedException();
    }

    Environment &environment;
    std::string name;
    Topology topology;
    uint64_t asynchronicity;
};

}

#endif
