#ifndef YAMPL_CHANNEL_H
#define YAMPL_CHANNEL_H

#include <string>

namespace yampl{

enum Context{
  THREAD = 0,
  LOCAL,
  DISTRIBUTED
};

struct Channel{
  public:
    Channel(const std::string &name, Context context = LOCAL) : name(name), context(context){}

    std::string name;
    Context context;
};

}

#endif
