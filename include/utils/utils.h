#ifndef YAMPL_UTILS_H
#define YAMPL_UTILS_H

#include <string>
#include <sstream>

namespace YAMPL{

template <typename T>
std::string to_string(T value){
  std::stringstream s;
  s << value;
  return s.str();
}

}

#endif
