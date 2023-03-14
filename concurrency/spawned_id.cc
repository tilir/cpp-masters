//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Spawned thread id
//
//----------------------------------------------------------------------------

#include <iostream>
#include <thread>

int main() {
  std::thread t([]() {
    std::cout << "Spawned id: " << std::this_thread::get_id() << std::endl;
  });
  std::cout << "Parent id: " << std::this_thread::get_id() << std::endl;
  t.join();
}
