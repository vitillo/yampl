#ifndef CHANNEL_H
#define CHANNEL_H

#include <string>
#include <stdint.h>

#include "Exceptions.h"

namespace IPC{

enum Topology{
  ONE_TO_ONE,
  ONE_TO_MANY,
  MANY_TO_ONE,
  MANY_TO_MANY
};

struct Channel{
    Channel(const std::string &name, Topology topology = ONE_TO_ONE, uint64_t asynchronicity = 1) : name(name), topology(topology), asynchronicity(asynchronicity){
      if(!asynchronicity)
	throw UnsupportedException();
    }

    std::string name;
    Topology topology;
    uint64_t asynchronicity;
};

}

#endif
