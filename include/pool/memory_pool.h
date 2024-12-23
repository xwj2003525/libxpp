#pragma once

#include "../ds/free_list.h"
#include <boost/noncopyable.hpp>
#include <memory>
#include <mutex>

namespace xpp {
namespace pool {

class MemoryPool_ts : boost::noncopyable {
protected:
  std::shared_ptr<ds::FreeList_ts> fl_;
  char *chunk;
  int size;
  std::mutex lock_;

public:
  MemoryPool_ts(std::shared_ptr<ds::FreeList_ts>);
  void *alloc(int &);
  void recycle(void *, int);
};
}; // namespace pool
}; // namespace xpp
