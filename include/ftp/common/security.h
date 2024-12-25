#pragma once

#include <cstddef>
#include <string>

namespace xpp {
namespace ftp {

namespace security {
bool encrypt(const char *, size_t, char *&, size_t &);
bool decrypt(const char *, size_t, char *&, size_t &);

}; // namespace security

}; // namespace ftp

}; // namespace xpp
