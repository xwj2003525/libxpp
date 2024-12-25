#pragma once

#include "../ds/task_queue.h"
#include <atomic>
#include <boost/noncopyable.hpp>
#include <functional>
#include <future>
#include <map>
#include <mutex>
#include <vector>
namespace xpp {
namespace pool {

class ThreadPool_ts : boost::noncopyable {
public:
  ThreadPool_ts();
  std::future<void> addTask(std::function<void()>);

  ~ThreadPool_ts();

protected:
  void run_resize();
  std::mutex lock_;
  bool running_;
  std::map<std::thread::id, std::thread> pool_;
  ds::TaskQueue_ts<std::packaged_task<void()>> queue_;
};

}; // namespace pool
}; // namespace xpp
