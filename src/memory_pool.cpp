#include "../include/pool/memory_pool.h"
#include <cstdlib>
#include <mutex>
#include <stdexcept>
#include <utility>

namespace xpp {

namespace pool {

MemoryPool_ts::MemoryPool_ts(std::shared_ptr<ds::FreeList_ts> f)
    : fl_(std::move(f)), chunk(nullptr), size(0) {
  if (!fl_) {
    throw std::runtime_error("invalid free list");
  }
}

void *MemoryPool_ts::alloc(int &bytes) {

  if (bytes < 1 || bytes > fl_->MAX_BYTES) {
    throw std::range_error("not valid range");
  }

  auto one = (*fl_).real_alloc_size(bytes);
  auto *ret = (*fl_).take(bytes);

  auto take_one_from_chunk = [this, one]() {
    auto *ret = chunk;
    chunk += one;
    size -= one;
    return ret;
  };

  // free list中没有
  if (!ret) {
    std::lock_guard<std::mutex> a(lock_);
    if (size >= one) {
      ret = take_one_from_chunk();
    } else {
      auto reuse_size = (*fl_).real_reuse_size(size);
      (*fl_).put(chunk, reuse_size);

      chunk = (char *)::malloc(size = 11 * one);
      ret = take_one_from_chunk();
    }

    if (size >= one) {
      (*fl_).put(take_one_from_chunk(), one);
    }
  }

  bytes = one;
  return ret;
}

void MemoryPool_ts::recycle(void *p, int bytes) { (*fl_).put(p, bytes); }
}; // namespace pool
}; // namespace xpp
