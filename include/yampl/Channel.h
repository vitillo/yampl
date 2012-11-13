#ifndef YAMPL_CHANNEL_H
#define YAMPL_CHANNEL_H

#include <string>

namespace yampl{

enum Context{
  THREAD = 0,
  LOCAL_PROCESS,
  DISTRIBUTED_PROCESS
};

struct Channel{
  public:
    Channel(const std::string &name, Context context = LOCAL_PROCESS) : name(name), context(context){}

    std::string name;
    Context context;
};

}

#endif
