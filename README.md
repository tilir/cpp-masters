# cpp-masters

C++ masters course revisited. Repo mostly contains examples, shaped as unit tests.

## Building and testing repo

Release build

```
cmake -G Ninja -B build -DCMAKE_CXX_COMPILER=g++-11 -DCMAKE_BUILD_TYPE=Release .
cmake --build build
env CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test
```

Debug build

```
cmake -G Ninja -B debug -DCMAKE_CXX_COMPILER=g++-11 -DCMAKE_BUILD_TYPE=Debug .
cmake --build debug
env CTEST_OUTPUT_ON_FAILURE=1 cmake --build debug --target test
```

