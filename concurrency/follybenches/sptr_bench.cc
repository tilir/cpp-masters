#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>
#include <shared_mutex>

#include <folly/Benchmark.h>
#include <folly/init/Init.h>
#include <gflags/gflags.h>

std::atomic<int> current_value{1};

DEFINE_bool(verify, false, "Enable correctness checks");

DEFINE_int32(readers,
             std::thread::hardware_concurrency() > 1
                 ? int(std::thread::hardware_concurrency() - 1)
                 : 1,
             "Number of reader threads");

DEFINE_int32(writers, 1, "Number of writer threads");

DEFINE_int32(writer_duration_ms, 2000,
             "Scenario duration (ms) for readers/writers");

DEFINE_int32(writer_pause_ns, 10,
             "Pause between writer updates (ns) outside critical section");

// ---------- API

struct alignas(std::hardware_destructive_interference_size) T {
  int x;
};

std::atomic<std::shared_ptr<T>> src = nullptr;

template <typename Fn> requires std::invocable<Fn&, const T*>
auto readAndAccess(Fn&& fn) {
  return std::invoke(std::forward<Fn>(fn), src.load().get());
}

void update(std::unique_ptr<T> newptr) {
  src.store(std::move(newptr));
}

// ----------

struct alignas(std::hardware_destructive_interference_size) ScenarioStats {
  std::uint64_t reads = 0;
  std::uint64_t failedChecks = 0;
};

void reset_state() {
  src.store(nullptr);
  current_value.store(1, std::memory_order_relaxed);
}

ScenarioStats run_mutex_scenario(std::size_t numReaders,
                                 std::size_t numWriters) {
  reset_state();

  std::atomic<bool> stop{false};
  std::vector<ScenarioStats> perReader(numReaders);

  std::vector<std::thread> readerThreads;
  readerThreads.reserve(numReaders);

  for (std::size_t i = 0; i < numReaders; ++i) {
    readerThreads.emplace_back([&, idx = i] {
      auto& local = perReader[idx];

      while (!stop.load(std::memory_order_relaxed)) {
        int v = readAndAccess([&](const T* ptr) {
          int local_v = 0;

          if (ptr) {
            local_v = ptr->x;

            if (FLAGS_verify) {
              int cur = current_value.load(std::memory_order_acquire);
              if (local_v < 1 || local_v > cur) {
                ++local.failedChecks;
              }
            }
          }

          ++local.reads;
          return local_v;
        });

        folly::doNotOptimizeAway(v);
      }
    });
  }

  std::vector<std::thread> writerThreads;
  writerThreads.reserve(numWriters);

  auto pause = std::chrono::nanoseconds(FLAGS_writer_pause_ns);

  for (std::size_t w = 0; w < numWriters; ++w) {
    writerThreads.emplace_back([&] {
      while (!stop.load(std::memory_order_relaxed)) {
        int next =
            current_value.fetch_add(1, std::memory_order_acq_rel);
        auto t = std::make_unique<T>(next);
        update(std::move(t));

        if (pause.count() > 0) {
          std::this_thread::sleep_for(pause);
        }
      }
    });
  }

  auto duration = std::chrono::milliseconds(FLAGS_writer_duration_ms);
  std::this_thread::sleep_for(duration);

  stop.store(true, std::memory_order_relaxed);

  for (auto& th : writerThreads)
    th.join();
  for (auto& th : readerThreads)
    th.join();

  ScenarioStats total{};
  for (auto& r : perReader) {
    total.reads += r.reads;
    total.failedChecks += r.failedChecks;
  }

  return total;
}

BENCHMARK(MutexRW, iters) {
  ScenarioStats agg{};

  const auto runs = iters;
  const std::size_t numReaders = static_cast<std::size_t>(FLAGS_readers);
  const std::size_t numWriters = static_cast<std::size_t>(FLAGS_writers);

  for (std::uint32_t i = 0; i < runs; ++i) {
    auto stats = run_mutex_scenario(numReaders, numWriters);
    agg.reads += stats.reads;
    agg.failedChecks += stats.failedChecks;
  }

  if (FLAGS_verify && agg.failedChecks != 0) {
    std::cerr << "[sptr] failed checks: " << agg.failedChecks << "\n";
    std::abort();
  }

  const double total_ms =
      static_cast<double>(runs) * static_cast<double>(FLAGS_writer_duration_ms);
  const double total_sec = total_ms / 1000.0;

  double rps_total = 0.0;
  double rps_per_reader = 0.0;

  if (total_sec > 0.0) {
    rps_total = static_cast<double>(agg.reads) / total_sec;
    if (numReaders > 0) {
      rps_per_reader =
          static_cast<double>(agg.reads) / (numReaders * total_sec);
    }
  }

  std::cout << "[sptr] bmk results\n";
  std::cout << "readers=" << numReaders << "\n";
  std::cout << "rps_per_reader=" << rps_per_reader << "\n";
}

int main(int argc, char** argv) {
  folly::Init init(&argc, &argv);
  folly::runBenchmarks();
}
