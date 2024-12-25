#pragma once

#include <boost/noncopyable.hpp>
#include <cstddef>

namespace xpp {
namespace ftp {

namespace session {
class Session : boost::noncopyable {
public:
  Session();
  ~Session();

  int socket() const;

  bool send(const char *, size_t) const;
  bool recv(char *, size_t) const;

private:
  const int client_socket_;
};

}; // namespace session

}; // namespace ftp

}; // namespace xpp
