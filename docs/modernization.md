# Toolchain modernization report

Validated locally on 2026-10-10, from baseline commit `5e4ecd2`.

## Result and validation

The project stays C++20. GCC 15 is the default for a fresh manual configure,
while explicit `CMAKE_CXX_COMPILER` and `CXX` remain respected. Four Ninja
presets provide GCC 15 / Clang 21 × Release / Debug; lecture order and the
original test/demo/source membership are preserved. `libatomic` is selected
by an actual compile/link probe. TBB is linked for the existing parallel
STL targets when available. The empty `install()` was removed.

| Preset | Build | ctest | New warnings |
|---|---|---|---|
| gcc-release | passed | 18/18 | 0 |
| gcc-debug | passed | 18/18 | 0 |
| clang-release | passed | 18/18 | 0 |
| clang-debug | passed | 18/18 | 0 |

Checks use `cmake --preset NAME`, `cmake --build --preset NAME`, and
`ctest --preset NAME --timeout 60`. No demo was executed. Both Debug cache
entries were checked to contain `CMAKE_BUILD_TYPE=Debug`; compile commands
use `-std=c++20`. A fresh manual configure without a compiler selected
`/usr/bin/g++-15`.

Warnings were compared by source basename and diagnostic text, excluding
line/column offsets and normalizing quote characters. Both sides use
`-Wall -Wextra`, identical compilers and build modes. The isolated baseline
contains HEAD sources, with only the compatibility repairs needed to finish
the build (the missing algorithm header, requires member declaration order,
and the two `handle_` typos), plus the same atomic/TBB build plumbing.
No warning suppressions were introduced.

| Toolchain | Baseline distinct diagnostics | Final distinct diagnostics |
|---|---:|---:|
| gcc-release | 61 | 61 |
| gcc-debug | 65 | 65 |
| clang-release | 95 | 95 |
| clang-debug | 95 | 95 |

`ruby tools/format.rb --check` passes with clang-format 21. IWYU uses
version 0.25 built against LLVM/Clang 21 (locally installed in ignored
`build/tools/iwyu`), checks built sources and reachable
project headers, and treats findings as errors. The standalone driver rejects
incompatible versions and non-Clang-21 databases; explicit database paths
are honored. `--error` replaces the old `--error_always`, which could never
pass even with clean includes. Apply mode keeps location comments required
by the upstream fixer and checks command failures. The normal standalone
`ruby tools/iwyu.rb --check` completed successfully using the local install;
apply mode was checked on a clean compilation-database subset. CMake IWYU
builds passed for `strings_unit`, `sfinae_unit`, `allocs_unit`, `smartptrs_unit`,
`concurrency_unit` and `parallel_unit`. Other built targets are covered by
the full standalone analysis.

CI defines the four build/test legs plus format and IWYU jobs. It explicitly
installs the compilers, Ruby, libraries, and builds IWYU 0.25 against LLVM 21.
The checkout action was updated to [v7](https://github.com/actions/checkout/releases).
IWYU’s [official release compatibility](https://include-what-you-use.org/downloads/)
identifies 0.25 with LLVM 21. Workflow YAML and matrix were checked locally.
The [first published IWYU job](https://github.com/tilir/cpp-masters/actions/runs/38064231638/job/114248507934)
failed before analyzing sources: the standalone
driver passed a bare executable name in `IWYU_BINARY`, but upstream's Python
driver resolves that override as a filesystem path, without searching `PATH`.
The failure was reproduced locally with a PATH-installed tool. The Ruby driver
now resolves the executable to an absolute real path before passing it on.
The full local IWYU check also covers this PATH-based invocation. A remote
rerun is still needed; local results do not assert a remote green run.

## Compatibility fixes in enabled examples

- `ranges/unreachable.cc`: include `<algorithm>` directly for `std::copy_n`.
- `ranges/nonsfinaeissue.cc`: declare `inner` before the requires expression;
  both new compilers reject accessing the undeclared member there. An inline
  comment explains that the non-SFINAE demonstration is preserved.
- `coroutines/natseq_range.cc`, `coroutines/natseq_zip.cc`: correct the
  `rhs.handle` typo to the existing `rhs.handle_`. GCC 15 checks the unused
  template body. The optional `BUG` branches remain unchanged; inline
  comments explain the compatibility change.
- `coroutines/subscribers_natlst.cc`: cache the linked awaiter’s next pointer
  before resuming. Clang may reuse the frame storage on the next `co_await`;
  reading the old awaiter after resume prevented this enabled test from
  completing correctly. This is an enabled positive example, with no broken
  variant or disabled-source marker. Other coroutine examples and their
  deliberately broken variants retain their behavior.
- `inference/jcase_positive.cc`: a narrow `no_forward_declare VIP` pragma
  suppresses a redundant declaration suggested for inherited constructors;
  the type is already fully defined. No source behavior changes.
- Buildable sources and reached utility/allocator headers include the standard
  headers they use directly. GTest macro expansions need `<string>` in many
  translation units. Public Boost Fusion/MPL wrappers replace implementation
  headers; the MPL preprocessed vector header cannot be included directly
  without its wrapper (GCC reported missing `LONG_MAX`).

## Formatting and IWYU exceptions

Excluded directories: `modules/`, `concurrency/follybenches/`,
`concurrency/tsv2/`. The formatter skips them and their local configuration
sets `DisableFormat: true`. No `.cppm` files were formatted. Standalone
projects and modules do not enter CMake or the IWYU database.

Formatting guards retain the instructional requires/concept layouts in:

- `concepts/compound.cc`: explained `clang-format off/on` region.
- `concepts/explicitreq.cc`: explained `clang-format off/on` region.
- `concepts/strange.cc`: explained `clang-format off/on` region.
- `lambdas/explicit.cc`: explained `clang-format off/on` region.
- `variadics/addable.cc`: explained `clang-format off/on` region.
- `variadics/addable2.cc`: explained `clang-format off/on` region.

Additional narrow IWYU exceptions:

- `inference/jcase_positive.cc`: `no_forward_declare VIP` rejects an unnecessary
  forward declaration for a type fully defined in the same file (inherited
  constructor attribution).
- `concurrency/sutter_case.cc`, `smartptrs/movable.cc`: `no_include <exception>` rejects an unused
  exception-pointer `swap` overload; the generic `std::swap` used here comes
  from the existing `<utility>` header.

Every explicit IWYU keep exception:

- `allocators/attempt1.cc`: `#include <limits> // IWYU pragma: keep -- preserve the original intentional teaching example.`
- `allocators/attempt1.cc`: `#include <list> // IWYU pragma: keep -- preserve the original intentional teaching example.`
- `allocators/attempt1.cc`: `#include <memory> // IWYU pragma: keep -- preserve the original intentional teaching example.`
- `allocators/attempt2.cc`: `#include <limits> // IWYU pragma: keep -- preserve the original intentional teaching example.`
- `allocators/attempt2.cc`: `#include <list> // IWYU pragma: keep -- preserve the original intentional teaching example.`
- `allocators/attempt2.cc`: `#include <memory> // IWYU pragma: keep -- preserve the original intentional teaching example.`
- `atomics/sptr_race.cc`: `#include <future> // IWYU pragma: keep -- preserve the original intentional race example.`
- `atomics/sptr_race.cc`: `#include <iostream> // IWYU pragma: keep -- preserve the original intentional race example.`
- `atomics/sptr_race.cc`: `#include <latch> // IWYU pragma: keep -- preserve the original intentional race example.`
- `atomics/sptr_race.cc`: `#include <utility> // IWYU pragma: keep -- preserve the original intentional race example.`
- `queues/classic_stack.cc`: `#include <iostream> // IWYU pragma: keep -- std::cout used in #if 0 debug dump below`
- `queues/classic_stack_wrong_demo.cc`: `#include <queue> // IWYU pragma: keep -- preserve the original intentional teaching example.`
- `sfinae/hasfoobar-naive.cc`: `#include <concepts> // IWYU pragma: keep -- preserve the original intentional teaching example.`

Mapping exceptions in `tools/iwyu.imp`:

- GTest/GMock internals map to their public umbrella headers.
- Boost MPL preprocessed `vector_c.hpp` maps to its public wrapper.
- Fusion templates map to their public `boost/fusion/include/` wrappers;
  adaptation macro implementation headers (including Boost.Preprocessor
  expansions) map to `adapt_struct.hpp`. These mappings retain the public API
  teaching example instead of adding dozens of macro implementation headers.
  The Preprocessor mapping is appropriate for the existing Fusion usage; new
  examples that use Boost.Preprocessor directly need their own public mapping.
- PSTL algorithm, numeric and memory internals map to their respective public
  headers; execution-policy and backend internals map to `<execution>`. The earlier broad PSTL mapping
  pulled TBB into `allocators/slist.hpp` and caused a GCC Debug link failure.
- TBB backend internals map to the public `<execution>` interface; C/C++
  header aliases and global size/ptrdiff types have explicit public mappings.
- `--no_fwd_decls` keeps complete definitions in instructional examples while
  preserving the missing/unused include checks. No whole-file IWYU skip is used.

## Teaching-example inventory

This inventory includes all commented-out source entries that resolve to
actual files, filename cues requested in `prompt.md`, and optional `BUG`
variants identified in the main tree. A cue is conservative: some naive
attempts or standalone demos are valid in the original build. Their original
membership is retained; exclusions are not silently enabled and existing
active examples are not silently disabled. All intentionally unsafe semantics,
compile-failure branches, races and hanging demos are preserved.

| File | Identification / existing status |
|---|---|
| `strings/cstrs.cc` | do not compile |
| `strings/fmt.cc` | requires GCC 13 |
| `functempls/generate-then-delete.cc` | supposed to fail |
| `functempls/nth-power-naive.cc` | supposed to fail; intentional teaching/name cue; original build membership retained |
| `classtemplates/danger1.cc` | intentional teaching/name cue; original build membership retained |
| `classtemplates/danger2.cc` | intentional teaching/name cue; original build membership retained |
| `sfinae/enableif-naive.cc` | supposed to fail, broken intentionally; intentional teaching/name cue; original build membership retained |
| `sfinae/fireonce-naive.cc` | supposed to fail, broken intentionally; intentional teaching/name cue; original build membership retained |
| `sfinae/hasfoobar-naive.cc` | intentional teaching/name cue; original build membership retained |
| `sfinae/logic-broken.cc` | supposed to fail, broken intentionally; intentional teaching/name cue; original build membership retained |
| `sfinae/st.cc` | commented-out source entry; original exclusion retained |
| `sfinae/void-discrim.cc` | supposed to fail, broken intentionally |
| `constexpr/cecollatz.cc` | commented-out source entry; original exclusion retained |
| `constexpr/cepred-expr.cc` | broken intentionally |
| `constexpr/ctconst.cc` | not a test |
| `constexpr/ctfield.cc` | broken intentionally |
| `constexpr/ctmap.cc` | commented-out source entry; original exclusion retained |
| `constexpr/heavy.cc` | just to look up the assembler |
| `constexpr/ifcedetails.cc` | broken intentionally |
| `constexpr/numlimits.cc` | broken intentionally |
| `constexpr/ubsan.cc` | broken intentionally to show how it works |
| `constexpr/virtce.cc` | broken intentionally |
| `concepts/contain.cc` | broken intentionally |
| `concepts/difference-complex-simple.cc` | optional BUG variant retained |
| `concepts/distance-too-simple.cc` | broken intentionally |
| `concepts/eq-complexreq.cc` | broken intentionally to show error processing |
| `concepts/eq-constexprif.cc` | broken intentionally to show error processing |
| `concepts/eq-enableif.cc` | broken intentionally to show error processing |
| `concepts/eq-require.cc` | broken intentionally to show error processing |
| `concepts/impossible.cc` | intentionally broken |
| `inference/jcase_broken_deftype.cc` | commented-out source entry; original exclusion retained; intentional teaching/name cue; original build membership retained |
| `inference/jcase_broken_fwd.cc` | commented-out source entry; original exclusion retained; intentional teaching/name cue; original build membership retained |
| `variadics/addable2.cc` | commented-out source entry; original exclusion retained |
| `variadics/commas.cc` | optional BUG variant retained |
| `variadics/cxxstyle_wrong.cc` | commented-out source entry; original exclusion retained; intentional teaching/name cue; original build membership retained |
| `variadics/expansion_wrong.cc` | commented-out source entry; original exclusion retained; intentional teaching/name cue; original build membership retained |
| `lambdas/fwd_capture.cc` | commented-out source entry; original exclusion retained |
| `lambdas/fwd_capture_astuple.cc` | commented-out source entry; original exclusion retained |
| `lambdas/fwd_capture_problem.cc` | must compile but fail execution for demo; documented unsafe/failing teaching example |
| `lambdas/generic_fail_demo.cc` | must fail for demo |
| `lambdas/transparent.cc` | this test will fail, it shows transparency problems |
| `lambdas/tricky_capture_demo.cc` | must fail for demo |
| `ranges/brokenfilter.cc` | commented-out source entry; original exclusion retained; intentional teaching/name cue; original build membership retained |
| `ranges/buggy-concat.cc` | requires separate build; documented unsafe/failing teaching example |
| `ranges/constfilter.cc` | commented-out source entry; original exclusion retained |
| `ranges/danglemessage.cc` | commented-out source entry; original exclusion retained; documented unsafe/failing teaching example |
| `ranges/dangling.cc` | documented unsafe/failing teaching example |
| `ranges/errmsg.cc` | commented-out source entry; original exclusion retained |
| `ranges/trimnaive.cc` | segfaults for gcc 11; intentional teaching/name cue; original build membership retained |
| `ranges/typed.cc` | requires separate build |
| `ranges/working-concat.cc` | requires separate build |
| `allocators/attempt1.cc` | intentional teaching/name cue; original build membership retained |
| `allocators/attempt2.cc` | intentional teaching/name cue; original build membership retained |
| `allocators/scopedalloc.cc` | problematic case, interesting CT error |
| `smartptrs/movable.cc` | optional BUG variant retained |
| `smartptrs/moveret_demo.cc` | just look at assembly |
| `smartptrs/prvalue.cc` | optional BUG variant retained |
| `runtimepoly/crtpcost_demo.cc` | just to look at assembly |
| `runtimepoly/dyncast_cost_demo.cc` | just to look at assembly |
| `runtimepoly/init_seq.cc` | optional BUG variant retained |
| `runtimepoly/sean_parents.cc` | executable demo |
| `runtimepoly/virtcost_demo.cc` | just to look at assembly |
| `runtimepoly/virtinh_cost_demo.cc` | just to look at assembly |
| `concurrency/deadlock.cc` | commented-out source entry; original exclusion retained; intentional teaching/name cue; original build membership retained |
| `concurrency/dining_naive.cc` | intentional teaching/name cue; original build membership retained |
| `concurrency/focus_demo.cc` | just demo |
| `concurrency/locations_ub.cc` | we have UB; intentional teaching/name cue; original build membership retained |
| `concurrency/posix_solution.cc` | we have UB |
| `concurrency/race_demo.cc` | we have UB; intentional teaching/name cue; original build membership retained |
| `concurrency/race_demo_volatile.cc` | we have UB; intentional teaching/name cue; original build membership retained |
| `concurrency/shared_lock_bench.cc` | benchmark |
| `queues/auto_return_wrong.cc` | must fail; intentional teaching/name cue; original build membership retained |
| `queues/classic_queue_demo.cc` | non-hanging bounded queue |
| `queues/classic_queue_wrong-01.cc` | intentional teaching/name cue; original build membership retained |
| `queues/classic_queue_wrong-02.cc` | intentional teaching/name cue; original build membership retained |
| `queues/classic_queue_wrong-03.cc` | intentional teaching/name cue; original build membership retained |
| `queues/classic_queue_wrong-04.cc` | intentional teaching/name cue; original build membership retained |
| `queues/classic_stack_demo.cc` | non-hanging |
| `queues/classic_stack_wrong_demo.cc` | hanging stack; intentional teaching/name cue; original build membership retained |
| `queues/unbounded_queue_demo.cc` | unbounded queue |
| `atomics/atomic_counter_demo.cc` | commented-out source entry; original exclusion retained |
| `atomics/atomic_twice_demo.cc` | commented-out source entry; original exclusion retained |
| `atomics/chasing_test.cc` | suppose to fail (rarely) |
| `atomics/custom_mutex_demo.cc` | commented-out source entry; original exclusion retained |
| `atomics/inc_naive.cc` | suppose to fail; intentional teaching/name cue; original build membership retained |
| `atomics/lockfree_queue.cc` | temporary switch off for unknown reasons (TODO: return it) |
| `atomics/lockfree_queue_wrong.cc` | supposed to fail; intentional teaching/name cue; original build membership retained |
| `atomics/sptr_race.cc` | intentional teaching/name cue; original build membership retained |
| `atomics/stack_reclaim_direct.cc` | suppose to fail |
| `atomics/stack_reclaim_problem.cc` | suppose to fail; documented unsafe/failing teaching example |
| `atomics/stack_reclaim_sptr.cc` | temporary switch off for unknown reasons (TODO: return it) |
| `atomics/stack_reclaim_sptr_wrong.cc` | supposed to fail; intentional teaching/name cue; original build membership retained |
| `coroutines/natseq_range.cc` | optional BUG variant retained |
| `coroutines/natseq_zip.cc` | optional BUG variant retained |
| `coroutines/subscribers_mt.cc` | shall fail |

## Changes to inventoried files

Each changed inventoried file is listed below. Formatting is allowed by the
request. Direct includes are mechanical dependencies, with inline explanations
on deliberately broken examples; keep pragmas retain their original includes.
No race, deadlock, UB, hang, or deliberately failing branch was repaired.

- `functempls/generate-then-delete.cc`: clang-format only.
- `functempls/nth-power-naive.cc`: clang-format only.
- `classtemplates/danger1.cc`: clang-format only.
- `classtemplates/danger2.cc`: clang-format only.
- `sfinae/enableif-naive.cc`: clang-format only.
- `sfinae/fireonce-naive.cc`: clang-format only.
- `sfinae/hasfoobar-naive.cc`: formatting; direct headers added: `#include <string>`; explained keep pragmas preserve original include choices.
- `sfinae/logic-broken.cc`: clang-format only.
- `sfinae/st.cc`: clang-format only.
- `sfinae/void-discrim.cc`: clang-format only.
- `constexpr/cepred-expr.cc`: clang-format only.
- `constexpr/ctmap.cc`: clang-format only.
- `constexpr/ifcedetails.cc`: clang-format only.
- `constexpr/numlimits.cc`: clang-format only.
- `constexpr/ubsan.cc`: clang-format only.
- `constexpr/virtce.cc`: clang-format only.
- `concepts/contain.cc`: clang-format only.
- `concepts/difference-complex-simple.cc`: formatting; direct headers added: `#include <string>`.
- `concepts/distance-too-simple.cc`: clang-format only.
- `concepts/eq-complexreq.cc`: clang-format only.
- `concepts/eq-constexprif.cc`: clang-format only.
- `concepts/eq-enableif.cc`: clang-format only.
- `concepts/eq-require.cc`: clang-format only.
- `concepts/impossible.cc`: clang-format only.
- `variadics/addable2.cc`: formatting; explained formatting guard preserves teaching layout.
- `variadics/commas.cc`: formatting; direct headers added: `#include <string>`, `#include <type_traits>`.
- `variadics/cxxstyle_wrong.cc`: clang-format only.
- `variadics/expansion_wrong.cc`: clang-format only.
- `lambdas/fwd_capture.cc`: clang-format only.
- `lambdas/fwd_capture_astuple.cc`: clang-format only.
- `lambdas/fwd_capture_problem.cc`: clang-format only.
- `lambdas/generic_fail_demo.cc`: clang-format only.
- `lambdas/transparent.cc`: clang-format only.
- `lambdas/tricky_capture_demo.cc`: clang-format only.
- `ranges/brokenfilter.cc`: clang-format only.
- `ranges/buggy-concat.cc`: clang-format only.
- `ranges/constfilter.cc`: clang-format only.
- `ranges/danglemessage.cc`: clang-format only.
- `ranges/dangling.cc`: formatting; direct headers added: `#include <iostream>`, `#include <string>`, `#include <type_traits>`.
- `ranges/errmsg.cc`: clang-format only.
- `ranges/trimnaive.cc`: clang-format only.
- `ranges/working-concat.cc`: clang-format only.
- `allocators/attempt1.cc`: formatting; direct headers added: `#include <string>`; explained keep pragmas preserve original include choices.
- `allocators/attempt2.cc`: formatting; direct headers added: `#include <string>`; explained keep pragmas preserve original include choices.
- `allocators/scopedalloc.cc`: clang-format only.
- `smartptrs/movable.cc`: formatting; direct headers added: `#include <string>`, `#include <utility>`; unused headers removed: `#include <algorithm>`, `#include <iostream>`, `#include <iterator>`, `#include <memory_resource>`.
- `smartptrs/prvalue.cc`: formatting; direct headers added: `#include <string>`; unused headers removed: `#include <algorithm>`, `#include <iostream>`, `#include <iterator>`, `#include <memory_resource>`.
- `runtimepoly/crtpcost_demo.cc`: clang-format only.
- `runtimepoly/init_seq.cc`: formatting; direct headers added: `#include <string>`; unused headers removed: `#include <iostream>`, `#include <memory>`, `#include <sstream>`.
- `runtimepoly/sean_parents.cc`: clang-format only.
- `concurrency/deadlock.cc`: clang-format only.
- `concurrency/dining_naive.cc`: formatting; direct headers added: `#include <chrono>`, `#include <functional>`, `#include <string>`.
- `concurrency/posix_solution.cc`: clang-format only.
- `concurrency/race_demo.cc`: clang-format only.
- `concurrency/shared_lock_bench.cc`: clang-format only.
- `queues/classic_queue_demo.cc`: formatting; direct headers added: `#include <functional>`, `#include <stdexcept>`, `#include <string>`; unused headers removed: `#include <queue>`.
- `queues/classic_queue_wrong-01.cc`: clang-format only.
- `queues/classic_queue_wrong-02.cc`: clang-format only.
- `queues/classic_queue_wrong-03.cc`: clang-format only.
- `queues/classic_queue_wrong-04.cc`: clang-format only.
- `queues/classic_stack_demo.cc`: formatting; direct headers added: `#include <functional>`, `#include <stdexcept>`, `#include <string>`; unused headers removed: `#include <stack>`.
- `queues/classic_stack_wrong_demo.cc`: formatting; direct headers added: `#include <functional>`, `#include <stdexcept>`, `#include <string>`; explained keep pragmas preserve original include choices.
- `queues/unbounded_queue_demo.cc`: formatting; direct headers added: `#include <functional>`, `#include <stdexcept>`, `#include <string>`.
- `atomics/atomic_counter_demo.cc`: formatting; direct headers added: `#include <stdexcept>`, `#include <string>`; unused headers removed: `#include <cassert>`, `#include <thread>`, `#include <vector>`.
- `atomics/atomic_twice_demo.cc`: formatting; direct headers added: `#include <stdexcept>`, `#include <string>`; unused headers removed: `#include <cassert>`, `#include <thread>`, `#include <vector>`.
- `atomics/chasing_test.cc`: clang-format only.
- `atomics/custom_mutex_demo.cc`: formatting; direct headers added: `#include <stdexcept>`, `#include <string>`; unused headers removed: `#include <cassert>`, `#include <vector>`.
- `atomics/inc_naive.cc`: clang-format only.
- `atomics/lockfree_queue.cc`: clang-format only.
- `atomics/lockfree_queue_wrong.cc`: clang-format only.
- `atomics/sptr_race.cc`: formatting; direct headers added: `#include <string>`; explained keep pragmas preserve original include choices.
- `atomics/stack_reclaim_direct.cc`: clang-format only.
- `atomics/stack_reclaim_problem.cc`: clang-format only.
- `atomics/stack_reclaim_sptr.cc`: clang-format only.
- `atomics/stack_reclaim_sptr_wrong.cc`: clang-format only.
- `coroutines/natseq_range.cc`: formatting; direct headers added: `#include <exception>`, `#include <iostream>`, `#include <iterator>`, `#include <stddef.h>`, `#include <string>`, `#include <utility>`; unused headers removed: `#include <future>`; unused move-assignment typo fixed for GCC 15; BUG branch untouched (see above).
- `coroutines/natseq_zip.cc`: formatting; direct headers added: `#include <exception>`, `#include <iterator>`, `#include <stddef.h>`, `#include <string>`, `#include <utility>`; unused headers removed: `#include <future>`; unused move-assignment typo fixed for GCC 15; BUG branch untouched (see above).
- `coroutines/subscribers_mt.cc`: clang-format only.

## Pre-existing failures deliberately retained

All excluded examples listed above stay excluded, including UB/race/deadlock
cases, failing lambda captures, allocator `scopedalloc.cc`, range filter and
concat failures, queue return errors and reclamation attempts. Sources omitted
because they are not on slides, require separate compilation, or have unknown
failure reasons remain omitted. `strings/fmt.cc` still has its original stale
exclusion reason; modernization does not expand course coverage.

`atomics/sptr_race.cc`, allocator attempts, and `classic_stack_wrong_demo`
already belonged to original build targets. Their teaching semantics and build
membership are preserved. Demo executables remain absent from ctest. Memory
tracking replacement-new examples still have their original limitations and
warnings; no allocator/atomic/coroutine redesign was introduced.

## Separate generated-artifact cleanup

The previously staged deletion of four committed module binaries was retained:

- `modules/extended/first_extended.pcm`
- `modules/extended/first_module.pcm`
- `modules/global-fragment/global.pcm`
- `modules/simplest/first_module.pcm`

`*.pcm` is ignored alongside `build/` and `debug/`. Module source files and
scripts are untouched. This cleanup is separate from compiler compatibility.
