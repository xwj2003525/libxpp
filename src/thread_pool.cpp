#include "../include/pool/thread_pool.h"
#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
namespace xpp {
namespace pool {

std::mutex a;
void out(int n) { std::cout << n << std::endl; }

static inline int min_thread() { return 1; }

static inline int max_thread() {
  auto ret = std::thread::hardware_concurrency();
  if (ret == 0)
    ret = 2;
  return ret;
}

void ThreadPool_ts::run_resize() {
  std::packaged_task<void()> func;
  int min = min_thread();
  int max = max_thread();

  int test = 0;
  auto id = std::this_thread::get_id();

  auto join_myself = [this, id]() {
    lock_.lock();
    auto it = this->pool_.find(id);
    if (it != pool_.end()) {
      if (it->second.joinable()) {
        it->second.join();
      }
      pool_.erase(it);
      out(pool_.size());
    }
    lock_.unlock();
  };

  while (running_) {
    {
      if (test <= -5) {
        lock_.lock();
        auto size = pool_.size();
        lock_.unlock();

        if (size > min) {
          addTask(join_myself);
          break; // join
        }

      } else if (test >= 5) {
        lock_.lock();
        auto size = pool_.size();
        if (size < max) {
          std::thread a(&ThreadPool_ts::run_resize, this);
          pool_.insert({a.get_id(), std::move(a)});

          out(pool_.size());
        }
        lock_.unlock();

        test = 0;
      }
    }

    auto ret = queue_.get_for(func, std::chrono::milliseconds(100));

    if (ret) {
      test++;
      func();
    } else {
      test--;
    }
  }
}

ThreadPool_ts::ThreadPool_ts() : running_(true) {
  int n = std::thread::hardware_concurrency();
  if (n == 0)
    n = 1;

  lock_.lock();
  for (int i = 0; i < n; i++) {
    std::thread a(&ThreadPool_ts::run_resize, this);
    pool_.insert({a.get_id(), std::move(a)});
  }
  lock_.unlock();
}

std::future<void> ThreadPool_ts::addTask(std::function<void()> func) {
  std::packaged_task<void()> task(func);
  auto f = task.get_future();
  queue_.add(std::move(task));

  return f;
}

ThreadPool_ts::~ThreadPool_ts() {
  lock_.lock();
  running_ = false;
  for (auto &[k, v] : pool_) {
    if (v.joinable()) {
      v.join();
    }
  }
}

}; // namespace pool
}; // namespace xpp
