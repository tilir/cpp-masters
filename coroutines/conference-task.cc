#include <generator>
#include <iostream>

 
template<typename T>
struct Task {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
 
    struct promise_type
    {
        std::exception_ptr exception_;
 
        Task get_return_object()
        {
            return Task(handle_type::from_promise(*this));
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() { exception_ = std::current_exception(); }

        void return_void() {}
    };
 
    handle_type h_;
 
    Task(handle_type h) : h_(h) {}
    ~Task() { h_.destroy(); }
    explicit operator bool() {
        return !h_.done();
    }

    void operator()() {
        h_();
    }
};


Task<int> coro_function(int n) {
    for (int i = 1; i < n; ++i) {
      printf("%d\n", i);
      co_await std::suspend_always{};
    }
}

int main() {
  auto Coro = coro_function(10);
  while(Coro)
    Coro();
}

