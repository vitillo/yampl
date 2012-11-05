#ifndef YAMPL_CHANNEL_H
#define YAMPL_CHANNEL_H

#include <string>

namespace YAMPL{

enum Topology{
  ONE_TO_ONE = 0,
  ONE_TO_MANY,
  MANY_TO_ONE,
  MANY_TO_MANY
};

enum Context{
  THREAD = 0,
  LOCAL_PROCESS,
  DISTRIBUTED_PROCESS
};

struct Channel{
  public:
    Channel(const std::string &name, Topology topology = ONE_TO_ONE, Context context = LOCAL_PROCESS) : name(name), topology(topology), context(context){}

    std::string name;
    Topology topology;
    Context context;
};

}

#endif
