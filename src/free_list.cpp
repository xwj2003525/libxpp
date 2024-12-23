#include "../include/ds/free_list.h"

namespace xpp {

namespace ds {

FreeList_ts::FreeList_ts(int b) : BYTES_INC(b), MAX_BYTES(16 * b) {
  if (b <= 0)
    throw std::range_error("not valid bytes inc");
  std::fill(std::begin(list_), std::end(list_), nullptr);
}

int FreeList_ts::real_alloc_size(int bytes) const {
  if (bytes < 1 || bytes > MAX_BYTES)
    return 0;

  return ((bytes + BYTES_INC - 1) / BYTES_INC) * BYTES_INC;
}

int FreeList_ts::real_reuse_size(int bytes) const {
  if (bytes < 1)
    return 0;
  return (bytes / BYTES_INC) * BYTES_INC;
}

static inline int index(int alloc, int inc) { return (alloc / inc) - 1; }
static inline int index_bytes(int index, int inc) { return (index + 1) * inc; }

void *FreeList_ts::take(int bytes) {
  auto alloc = real_alloc_size(bytes);
  if (alloc == 0)
    return nullptr;

  auto i = index(alloc, BYTES_INC);
  std::lock_guard<std::mutex> a(lock_);
  auto *ret = list_[i];
  if (ret == nullptr) {

    // 找一块大的分为两块
    for (i++; i < 16; i++) {
      if (list_[i]) {
        ret = list_[i];
        list_[i] = list_[i]->next;
        auto rest = index_bytes(i, BYTES_INC) - alloc;

        // 如果有剩余
        if (rest > 0) {
          auto j = index(rest, BYTES_INC);
          Node *sub = (Node *)((char *)ret + alloc);
          sub->next = list_[j];
          list_[j] = sub;
        }
        break;
      }
    }
  }

  return ret;
}

void FreeList_ts::put(void *p, int bytes) {
  auto use = real_reuse_size(bytes);
  if (use == 0)
    return;

  Node *q = (Node *)p;
  std::lock_guard<std::mutex> a(lock_);
  for (; use > MAX_BYTES; use -= MAX_BYTES) {
    q->next = list_[15];
    list_[15] = q;
    q = (Node *)((char *)q + MAX_BYTES);
  }

  auto i = index(use, BYTES_INC);
  q->next = list_[i];
  list_[i] = q;
}

}; // namespace ds

}; // namespace xpp
