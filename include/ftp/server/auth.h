#pragma once

#include <SQLiteCpp/Database.h>
#include <boost/noncopyable.hpp>
#include <string>
namespace xpp {
namespace ftp {
namespace Ser {

class Auth : boost::noncopyable {
public:
  Auth(const std::string &);
  bool findUser(const std::string &username) const;
  void deleteUser(const std::string &username) const;
  bool verifyUser(const std::string &username,
                  const std::string &password) const;
  void updateUser(const std::string &username,
                  const std::string &newPassword) const;

private:
  SQLite::Database db;
};

}; // namespace Ser

}; // namespace ftp
}; // namespace xpp
