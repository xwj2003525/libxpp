#include "include/pool/thread_pool.h"

#include <future>
#include <iostream>
#include <mutex>
#include <thread>

xpp::pool::ThreadPool_ts t;

void a() {}

int main(int argc, char *argv[]) {

  while (1) {
    t.addTask(a);
  }

  return 0;
}
