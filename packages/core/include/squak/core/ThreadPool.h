#pragma once
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>

class ThreadPool {
  using Executable = std::function<void(void)>;
  bool terminated = false;
  size_t working = 0;
  std::vector<std::thread> threads{};
  std::mutex mutex{};
  std::condition_variable condition{};
  std::queue<Executable> queue{};

public:
  ThreadPool(size_t count);
  ~ThreadPool();
  void Run(Executable executable);
  bool AtCapacity() { return this->working = threads.size(); }
  bool IsTerminated() { return this->terminated; }
};