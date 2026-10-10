# AGENTS.md

Guidance for AI agents (and humans) working in this repository.

## What this repo is

Example code for the MIPT masters course on C++
(slides: https://sourceforge.net/projects/cpp-lects-rus), licensed GPL v3.
Examples are shaped as GoogleTest unit tests, grouped one directory per
lecture, in lecture order:

strings, functempls, classtemplates, (modules), sfinae, constexpr, concepts,
inference, variadics, lambdas, ranges, allocators, smartptrs, runtimepoly,
concurrency, queues, atomics, parallel, coroutines.

Preserve this order in build files and docs. Every source file starts with a
GPL banner comment — never remove banners.

## CRITICAL: intentionally broken examples

Many examples are pedagogically wrong ON PURPOSE. Never "fix" them.

Recognize them by:

- commented-out entries in per-lecture `SRCS` lists, tagged `-- <reason>`
  (the CMake header comment says: "Examples commented out are broken (often
  -- intentionally)");
- file naming: `*_wrong*.cc`, `*-broken.cc`, `broken*`, `*_naive.cc`,
  `danger*.cc`, `locations_ub.cc`, `race_demo*.cc`, `deadlock.cc`,
  `jcase_broken_*.cc`, `attempt1.cc` / `attempt2.cc`, etc.;
- demos that intentionally hang or exhibit UB (e.g.
  `classic_stack_wrong_demo`, `race_demo.cc`).

Rules:

- NEVER fix their semantics. Do not uncomment them into the build. Do not
  repair UB, races, deadlocks, or compile errors that are the teaching point.
- Mechanical, semantics-preserving adjustments required by new toolchains
  (e.g. a moved standard header) are allowed; add a comment explaining the
  edit and mention it in the change report.
- Broken examples must never break the default build, ctest, or CI.
- `modules/` is not part of the CMake build (driven by `runme.sh` scripts);
  leave it out of build/CI/IWYU/format integration. Do not reformat `.cppm`
  files.
- `concurrency/follybenches/` and `concurrency/tsv2/` are standalone (extra
  deps) and stay out of the default build, IWYU, and clang-format.

## Building and testing

Compilers: gcc-15 (default) or clang-21. CMake >= 3.21, Ninja, GTest,
Boost, TBB (used by PSTL when available).

Presets (Ninja, build trees under `build/<preset>`):

```
cmake --preset gcc-release      # or gcc-debug, clang-release, clang-debug
cmake --build --preset gcc-release
ctest --preset gcc-release      # output-on-failure is preset
```

Manual equivalent (kept working, any build dir name):

```
cmake -G Ninja -B build -DCMAKE_CXX_COMPILER=g++-15 -DCMAKE_BUILD_TYPE=Release .
cmake --build build
env CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test
```

For clang: `-DCMAKE_CXX_COMPILER=clang++-21`.

The standard stays C++20 — do not raise it; the course content is C++20.

Demos vs tests: `add_demo` targets are built but NOT registered as ctest
tests — several demos hang or run benchmarks by design. Never run demo
binaries in CI.

libatomic is linked automatically (and only) when the compiler cannot inline
struct atomics — see `CPM_HAVE_BUILTIN_WIDE_ATOMICS` in the root
CMakeLists.txt.

## Adding examples

- New testable example: add the file to the right lecture's `SRCS` list in
  lecture order; it becomes part of `<lecture>_unit` / `<lecture>_test`.
- Non-test demo: `add_demo(name)` in the lecture CMakeLists (builds
  `name.cc`, not run by ctest).
- Intentionally broken example: commit the file but keep its `SRCS` entry
  commented out with a `-- <reason>` tag. It must never enter the default
  build.

## Tooling policy

Warnings: `-Wall -Wextra` via the `cpm_warnings` interface target, no
`-Werror`. Examples are pedagogical and trigger warnings on purpose; warnings
are informational and CI does not gate on them, but a change must not add new
warnings beyond the recorded baseline. Prefer per-target suppressions (with a
comment) over touching pedagogical code.

Formatting: clang-format-21, config in the root `.clang-format` (Google base,
right pointer alignment, preserved include blocks). Run
`ruby tools/format.rb` to format, `ruby tools/format.rb --check` to verify
(this is the CI gate). Excluded: `modules/`, `concurrency/follybenches/`,
`concurrency/tsv2/` (per-directory `DisableFormat`). If reformatting would
destroy a teaching point (e.g. carefully aligned code), wrap it in
`// clang-format off` / `// clang-format on` with a comment.

IWYU: version 0.25 built against LLVM/Clang 21, with `tools/iwyu.imp`
mappings (GTest, public Boost Fusion/MPL interfaces, and C/C++ headers).
Do not install an unversioned distro IWYU and assume it matches Clang 21.
Use `IWYU_BINARY` to choose the standalone executable or `CPM_IWYU_EXE`
for CMake. Both drivers discover a local install in `build/tools/iwyu/bin`.
The CMake option rejects an incompatible IWYU version.
- CMake route: `cmake --preset clang-release -DCPM_ENABLE_IWYU=ON` then build.
- Standalone: `ruby tools/iwyu.rb --check` (CI gate) /
  `ruby tools/iwyu.rb` to apply suggestions via `fix_include`.
- Suppression convention: `// IWYU pragma: keep` (or `export`) with a short
  comment when an include is pedagogically intentional; prefer that over
  weakening the mapping file. IWYU covers targets in the CMake build and
  project headers reached through them. `--no_fwd_decls` keeps examples using
  their complete type definitions; it does not disable checks for missing headers.
- Upstream installs `iwyu_tool.py` and `fix_includes.py`: select them with
  `IWYU_TOOL` and `FIX_INCLUDE`. Default distro names are `iwyu_tool` and
  `fix_include`. Apply mode preserves comments and fails on analysis errors.
- Preserve unused original includes in intentionally broken examples with
  explained `keep` pragmas. Enumerate exceptions in `docs/modernization.md`.
- Some unsafe examples (notably `atomics/sptr_race.cc` and allocator attempts)
  already belong to the original test targets; preserve their membership and
  semantics. Do not register hanging demos as tests.

CI (`.github/workflows/workflow.yml`): matrix {gcc-15, clang-21} x
{Release, Debug} — configure, build, ctest with output-on-failure — plus
`format` and `iwyu` jobs. All legs must stay green.
