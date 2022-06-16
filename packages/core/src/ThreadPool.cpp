#include <squak/core/ThreadPool.h>

ThreadPool::ThreadPool(size_t size) {
  for (size_t i = 0; i < size; i++) {
    this->threads.push_back(std::thread{ [this]() {
      while (true) {
        std::unique_lock<std::mutex> lock(this->mutex);
        while (!queue.size()) {
          if (this->terminated) {
            return;
          }

          this->condition.wait(lock);
        }

        Executable executable = queue.front();
        queue.pop();
        this->working++;
        
        lock.unlock();

        executable();

        lock.lock();
        this->working--;
      }
    } });
  }
}

void ThreadPool::Run(Executable executable) {
  {
    std::unique_lock<std::mutex> lock(this->mutex);
    this->queue.push(executable);
  }
 
  this->condition.notify_one();
}

ThreadPool::~ThreadPool() {
  this->terminated = true;
  this->condition.notify_all();
  for (auto& thread : this->threads) {
    thread.join();
  }
}