#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include <folly/synchronization/Rcu.h>

std::mutex cout_mutex;

struct Data {
  int value;
  explicit Data(int v) : value(v) {}
  ~Data() {
    std::scoped_lock l{cout_mutex};
    std::cout << "Dtor for " << value << "\n"; 
  }
};

std::atomic<int> current;
std::atomic<Data*> g_data{nullptr};

void reader_thread(int id, std::atomic<bool>& stop) {
  while (!stop) {
    std::scoped_lock<folly::rcu_domain> guard{folly::rcu_default_domain()};
    Data* ptr = g_data;
    if (ptr->value != current) {
      std::scoped_lock l{cout_mutex};
      std::cout << "[reader " << id << "] value = " << ptr->value << " not " << current << "\n";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

int main() {
  current = 1;
  g_data = new Data{1};
  std::atomic<bool> stop = false;
  std::vector<std::thread> readers;
  readers.reserve(4);

  for (int i = 0; i < 4; ++i)
    readers.emplace_back(reader_thread, i, std::ref(stop));

  for (int v = 2; v <= 5; ++v) {
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    current = v;
    auto* newPtr = new Data{v};
    Data* oldPtr = g_data.exchange(newPtr, std::memory_order_relaxed);
    folly::rcu_retire(oldPtr);
    {
      std::scoped_lock l{cout_mutex};
      std::cout << "[writer] updated value to " << v << "\n";
    }
  }

  stop = true;
  for (auto& t : readers)
    t.join();

  folly::rcu_synchronize();

  Data* last = g_data;
  if (last) {
    folly::rcu_retire(last);
    folly::rcu_synchronize();
  }

  std::cout << "Done.\n";
}

