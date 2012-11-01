#ifndef IPC_UTILS_H
#define IPC_UTILS_H

#include <string>
#include <sstream>

namespace IPC{

template <typename T>
std::string to_string(T value){
  std::stringstream s;
  s << value;
  return s.str();
}

}

#endif
