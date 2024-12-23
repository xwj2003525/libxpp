#include "include/pool/thread_pool.h"
#include <iostream>
#include <thread>
void a() { std::cout << std::this_thread::get_id() << std::endl; }

int main(int argc, char *argv[]) {
  xpp::pool::ThreadPool_ts b;

  b.addTask(a);

  b.addTask(a);
  b.addTask(a);
  b.addTask(a);
  return 0;
}
