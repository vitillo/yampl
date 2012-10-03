#include <unistd.h>

#include <cstring>
#include <sstream>

inline std::string pid(){
  std::stringstream s;
  s << getpid();
  return s.str();
}

inline void deallocator(void *, void*){}

