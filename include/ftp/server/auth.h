#pragma once

#include <string>
namespace xpp {

namespace ftp {

namespace auth {
bool log_in(const std::string &, const std::string &);
bool sign_up(const std::string &, const std::string &);
bool user_del(const std::string &);
bool user_modify(const std::string &, const std::string &);
bool user_exist(const std::string &);

}; // namespace auth

}; // namespace ftp

}; // namespace xpp
