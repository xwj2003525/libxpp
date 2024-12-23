#pragma once

#include <boost/noncopyable.hpp>
#include <mutex>

namespace xpp {
namespace ds {
class FreeList_ts : boost::noncopyable {
public:
  FreeList_ts(int bytesInc);

  void *take(int bytes);
  void put(void *, int bytes);

  int real_alloc_size(int) const;
  int real_reuse_size(int) const;

  const int BYTES_INC;
  const int MAX_BYTES;

protected:
  std::mutex lock_;

  struct Node {
    Node *next = nullptr;
  };

  Node *list_[16];
};
}; // namespace ds
}; // namespace xpp
