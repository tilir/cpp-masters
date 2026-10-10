# cpp-masters

[![build-and-test](https://github.com/tilir/cpp-masters/actions/workflows/workflow.yml/badge.svg)](https://github.com/tilir/cpp-masters/actions/workflows/workflow.yml)

Example code for the MIPT masters course on modern C++. Course slides:
https://sourceforge.net/projects/cpp-lects-rus. Licensed under GNU GPL v3.

Examples are shaped as GoogleTest unit tests, grouped one directory per
lecture.

> **Warning: many examples are intentionally broken for teaching purposes.**
> They demonstrate UB, races, deadlocks, compile errors and hangs — for
> instance `queues/classic_queue_wrong-01.cc`, `concurrency/race_demo.cc`
> and `concurrency/deadlock.cc`. Compile-failing examples are excluded from `SRCS`. Some unsafe
> demonstrations already belong to the build, including `sptr_race.cc`;
> hanging demos are built but never registered with ctest.
> Do not "fix" them; see [AGENTS.md](AGENTS.md) for the full policy.

## Prerequisites

- gcc-15 (default) or clang-21
- cmake >= 3.21, ninja; Ruby and Python 3 for the tooling scripts
- GTest, Boost, TBB (TBB is used by parallel STL when present)

Debian/Ubuntu:

```
sudo apt-get install g++-15 ninja-build libgtest-dev google-mock libboost-dev libtbb-dev
```

## Quick start

Preset-based (recommended; build trees land in `build/<preset>`):

```
cmake --preset gcc-release
cmake --build --preset gcc-release
ctest --preset gcc-release
```

Clang quick start:

```
cmake --preset clang-release
cmake --build --preset clang-release
ctest --preset clang-release
```

Available presets: `gcc-release`, `gcc-debug`, `clang-release`, `clang-debug`.

Manual equivalent with the default compiler:

```
cmake -G Ninja -B build -DCMAKE_CXX_COMPILER=g++-15 -DCMAKE_BUILD_TYPE=Release .
cmake --build build
env CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test
```

Switch to clang with `-DCMAKE_CXX_COMPILER=clang++-21`, using a separate build
directory. For the existing Debug workflow:

```
cmake -G Ninja -B debug -DCMAKE_CXX_COMPILER=g++-15 -DCMAKE_BUILD_TYPE=Debug .
cmake --build debug
ctest --test-dir debug --output-on-failure
```

### Demos

Some lectures also build demo binaries (`add_demo` targets). They are built
but never run by ctest: several of them hang or run benchmarks by design
(e.g. `classic_stack_wrong_demo`). Run them manually, at your own risk, from
the build tree.

For example, build and run the bounded queue demo:

```
cmake --build --preset gcc-release --target classic_queue_demo
build/gcc-release/queues/classic_queue_demo -help
```

The C++ standard is pinned to C++20 on purpose — the course content is C++20.

## Lecture map (in course order)

| Directory | Topic |
|---|---|
| `strings/` | strings, C-string conversions, tokenizing |
| `functempls/` | function templates, deduction, specialization |
| `classtemplates/` | class templates, partial specialization, deduction guides |
| `modules/` | C++20 modules (not in the CMake build; see its `runme.sh` scripts) |
| `sfinae/` | SFINAE, `enable_if`, detection idiom |
| `constexpr/` | `constexpr` / `consteval`, compile-time evaluation |
| `concepts/` | C++20 concepts and `requires` |
| `inference/` | type inference, reference collapsing, forwarding |
| `variadics/` | variadic templates, fold expressions |
| `lambdas/` | lambdas, captures, generic lambdas |
| `ranges/` | ranges, views, sentinels |
| `allocators/` | allocators, `pmr` |
| `smartptrs/` | smart pointers, move semantics |
| `runtimepoly/` | virtual functions, CRTP, `dynamic_cast` |
| `concurrency/` | threads, mutexes, condition variables |
| `queues/` | thread-safe queues, futures, latches |
| `atomics/` | atomics, lock-free structures |
| `parallel/` | parallel STL, `std::async`, reductions |
| `coroutines/` | C++20 coroutines |

## Contributing

- Add examples to the right lecture's `SRCS` in lecture order; intentionally
  broken ones stay commented out with a `-- <reason>` tag.
- Format with clang-format-21: `ruby tools/format.rb` (check:
  `ruby tools/format.rb --check`).
- IWYU: `ruby tools/iwyu.rb --check`, mappings in `tools/iwyu.imp`.
- Details and the broken-examples policy: [AGENTS.md](AGENTS.md).

IWYU must be **0.25 built against LLVM/Clang 21**; the distribution’s unversioned
`iwyu` package may use an older LLVM. Build the [0.25 release](https://include-what-you-use.org/downloads/)
with `-DCMAKE_PREFIX_PATH=/usr/lib/llvm-21`. A repository-local install in
`build/tools/iwyu` is discovered automatically by both drivers:

```
cmake --install /path/to/iwyu-build --prefix "$PWD/build/tools/iwyu"
ruby tools/iwyu.rb --check
```

Alternatively install to another local prefix and select its executable with
`IWYU_BINARY`.
For upstream’s executable names, use:

```
IWYU_TOOL=iwyu_tool.py FIX_INCLUDE=fix_includes.py ruby tools/iwyu.rb --check
IWYU_TOOL=iwyu_tool.py FIX_INCLUDE=fix_includes.py ruby tools/iwyu.rb
ruby tools/format.rb
```

`IWYU_BINARY` can select a locally built executable; `IWYU_JOBS` controls parallelism.
The driver checks built translation units and the project headers they include.
See [the modernization report](docs/modernization.md) for preserved teaching
examples, tooling exceptions, and validation results.
