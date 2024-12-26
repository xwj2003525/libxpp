#pragma once

#include <boost/noncopyable.hpp>
#include <cstdint>
#include <string>
#include <vector>

namespace xpp {
namespace ftp {

namespace protocol {
constexpr int COMMAND = 1;
constexpr int FILE_START = 2;
constexpr int FILE_PART = 4;
constexpr int FILE_END = 8;
}; // namespace protocol

namespace ascii {
void ascii_to_windows(std::string &);
void ascii_to_unix(std::string &);
}; // namespace ascii

namespace Security {
std::string encrypt(const std::string &);
std::string decrypt(const std::string &);
std::vector<uint8_t> encrypt(const std::vector<uint8_t> &);
std::vector<uint8_t> decrypt(const std::vector<uint8_t> &);
}; // namespace Security

namespace session {
class Session : boost::noncopyable {
public:
  // create a socket when session create
  Session();
  ~Session();
  int socket() const;

private:
  const int client_socket_;
};

class SerSession : public Session {
public:
  void run() const;
};

class CliSession : public Session {
public:
  void run() const;
};

}; // namespace session

}; // namespace ftp

}; // namespace xpp
