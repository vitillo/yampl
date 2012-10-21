#ifndef CHANNEL_H
#define CHANNEL_H

#include <string>

namespace IPC{

enum Topology{
  ONE_TO_ONE,
  ONE_TO_MANY,
  MANY_TO_ONE,
  MANY_TO_MANY
};

struct Channel{
  public:
    Channel(const std::string &name, Topology topology = ONE_TO_ONE) : name(name), topology(topology){}

    std::string name;
    Topology topology;
};

}

#endif
