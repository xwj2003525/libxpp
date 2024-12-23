#include "../include/pool/thread_pool.h"
#include <exception>
#include <functional>
#include <future>
#include <thread>
namespace xpp {
namespace pool {

void run(bool &running, ds::TaskQueue_ts<std::packaged_task<void()>> &q) {
  std::packaged_task<void()> func;
  while (running) {
    auto ret = q.get_for(func, std::chrono::milliseconds(100));
    if (ret) {
      func();
    }
  }
}

ThreadPool_ts::ThreadPool_ts() : running_(true) {
  int n = std::thread::hardware_concurrency();
  if (n == 0)
    n = 2;

  for (int i = 0; i < n; i++) {
    pool_.emplace_back(run, std::ref(this->running_), std::ref(this->queue_));
  }
}

std::future<void> ThreadPool_ts::addTask(std::function<void()> func) {
  std::packaged_task<void()> task(func);
  auto f = task.get_future();
  queue_.add(std::move(task));

  return f;
}

ThreadPool_ts::~ThreadPool_ts() {
  running_ = false;
  for (auto &i : pool_) {
    if (i.joinable()) {
      i.join();
    }
  }
}

}; // namespace pool
}; // namespace xpp
