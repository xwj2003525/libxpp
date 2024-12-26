#include "../include/ftp/server/auth.h"

namespace xpp {
namespace ftp {
namespace Ser {

Auth::Auth(const std::string &dbName)
    : db(dbName, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {
  try {
    db.exec("CREATE TABLE IF NOT EXISTS USER (ID INTEGER PRIMARY KEY, USERNAME "
            "TEXT UNIQUE, PASSWORD TEXT)");
  } catch (const SQLite::Exception &e) {
    throw std::runtime_error("Failed to create user table : " +
                             std::string(e.what()));
  }
}

bool Auth::findUser(const std::string &username) const {
  try {
    SQLite::Statement query(db, "SELECT COUNT(*) FROM USER WHERE USERNAME = ?");
    query.bind(1, username);

    if (query.executeStep()) {
      return query.getColumn(0).getInt() > 0;
    }
  } catch (const SQLite::Exception &e) {
    throw std::runtime_error("Failed to find user : " + std::string(e.what()));
  }
  return false;
}

void Auth::deleteUser(const std::string &username) const {
  try {
    SQLite::Statement query(db, "DELETE FROM USER WHERE USERNAME = ?");
    query.bind(1, username);
    query.exec();
  } catch (const SQLite::Exception &e) {
    throw std::runtime_error("Failed to delete user : " +
                             std::string(e.what()));
  }
}

bool Auth::verifyUser(const std::string &username,
                      const std::string &password) const {
  try {
    SQLite::Statement query(db, "SELECT PASSWORD FROM USER WHERE USERNAME = ?");
    query.bind(1, username);

    if (query.executeStep()) {
      std::string storedPassword = query.getColumn(0).getText();
      return storedPassword == password;
    }
  } catch (const SQLite::Exception &e) {
    throw std::runtime_error("Failed to verify user : " +
                             std::string(e.what()));
  }
  return false;
}

void Auth::updateUser(const std::string &username,
                      const std::string &newPassword) const {
  try {
    auto ret = findUser(username);
    if (!ret)
      return;

    SQLite::Statement query(db,
                            "UPDATE USER SET PASSWORD = ? WHERE USERNAME = ?");
    query.bind(1, newPassword);
    query.bind(2, username);
    query.exec();
  } catch (const SQLite::Exception &e) {
    throw std::runtime_error("Failed to update user : " +
                             std::string(e.what()));
  }
}

}; // namespace Ser
}; // namespace ftp
}; // namespace xpp
