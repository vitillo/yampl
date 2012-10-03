#include "Socket.h"

namespace IPC{

void defaultDeallocator(void *buffer, void *hint){
  free(buffer);
}

}
