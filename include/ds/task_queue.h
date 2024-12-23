#pragma once

#include <chrono>
#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
namespace xpp {

namespace ds {

template <class T> class TaskQueue_ts {
public:
  void add(T &&);
  void get(T &);
  bool try_get(T &);
  bool get_for(T &, std::chrono::milliseconds);

protected:
  std::queue<T> queue_;
  std::condition_variable cv_;
  std::mutex lock_;
};

template <class T> void TaskQueue_ts<T>::add(T &&task) {
  std::lock_guard<std::mutex> a(lock_);
  queue_.push(std::move(task));
  cv_.notify_one();
}

template <class T> void TaskQueue_ts<T>::get(T &task) {
  std::unique_lock<std::mutex> a(lock_);
  cv_.wait(a, [this, &task]() { return !queue_.empty(); });
  task = std::move(queue_.front());
  queue_.pop();
}

template <class T>
bool TaskQueue_ts<T>::get_for(T &task, std::chrono::milliseconds m) {
  if (m == std::chrono::milliseconds(0))
    return false;

  std::unique_lock<std::mutex> a(lock_);
  auto ret = cv_.wait_for(a, m, [this, &task]() { return !queue_.empty(); });
  if (ret) {
    task = std::move(queue_.front());
    queue_.pop();
  }
  return ret;
}

template <class T> bool TaskQueue_ts<T>::try_get(T &task) {
  std::unique_lock<std::mutex> a(lock_);
  if (!queue_.empty()) {
    task = std::move(queue_.front());
    queue_.pop();
    return true;
  }

  return false;
}

}; // namespace ds

}; // namespace xpp
