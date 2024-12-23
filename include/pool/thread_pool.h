#pragma once

#include "../ds/task_queue.h"
#include <boost/noncopyable.hpp>
#include <functional>
#include <future>
#include <thread>
#include <vector>
namespace xpp {
namespace pool {

class ThreadPool_ts : boost::noncopyable {
public:
  ThreadPool_ts();
  std::future<void> addTask(std::function<void()>);

  ~ThreadPool_ts();

protected:
  bool running_;
  std::vector<std::thread> pool_;
  ds::TaskQueue_ts<std::packaged_task<void()>> queue_;
};

}; // namespace pool
}; // namespace xpp
