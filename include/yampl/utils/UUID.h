#ifndef YAMPL_UUID_H
#define YAMPL_UUID_H

#include <uuid/uuid.h>

#include <string>
#include <cmath>

#include "yampl/utils/RawPipe.h"
#include "yampl/Exceptions.h"

namespace yampl{

class UUID{
  friend std::ostream & operator<<(std::ostream &, const UUID &);

  public:
    UUID(const std::string &id);

    void writeTo(RawPipe &pipe);
    operator std::string();

    static UUID readFrom(RawPipe &pipe);

  private:
    static const size_t s_size = 37;
    std::string m_id;
};

inline std::ostream & operator<<(std::ostream &stream, const UUID &uuid){
  stream << uuid.m_id;
  return stream;
}

inline UUID::UUID(const std::string &id){
  if(id.empty()){
    uuid_t uuid;
    char out[s_size];

    uuid_generate(uuid);
    uuid_unparse(uuid, out);
    m_id = out;
  }else{
    if(m_id.size() > s_size){
      throw InvalidOperationException("ID too long");
    }

    m_id = id;
  }
}

inline void UUID::writeTo(RawPipe &pipe){
  char out[s_size] = {0};

  memcpy(out, m_id.c_str(), std::min(s_size, m_id.size()));
  pipe.write(out, s_size);
}

inline UUID::operator std::string(){
  return m_id;
}

inline UUID UUID::readFrom(RawPipe &pipe){
  char out[s_size];

  pipe.read(out, s_size);
  return UUID(out);
}

}

#endif
