#pragma once
#include <string>
#include <vector>

namespace xpp {
namespace ftp {

namespace File {

enum Type { Directory, ASCII_FILE, BINARY_FILE };

struct Info {
  std::string name_;
  std::string prefix_dir_;
  std::string owner_;
  std::uint64_t create_timestamp_;
  std::uint64_t last_modify_timestamp_;
  std::uint64_t last_access_timestamp_;
  Type type_;
  std::uint64_t bytes_;
  std::string permission_;
};

}; // namespace File

class Dir {
public:
  Dir(File::Info);
  Dir(const std::string &);

  bool pwd(std::string &) const;
  bool cd(const std::string &);
  bool mv(const std::string &, const std::string &) const;
  bool ls(const std::string &, std::vector<File::Info> &) const;

private:
  std::string current_dir_;
};

}; // namespace ftp
}; // namespace xpp
