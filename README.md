# cpp-masters

C++ masters course revisited. Repo mostly contains examples, shaped as unit tests.

## Building and testing repo

You will need at least g++-12 because some examples are using atomic<shared_ptr>

Full prerequisite list

```
sudo apt-get install libgtest-dev googletest googletest-tools google-mock libboost-dev g++-12 ninja-build
```

Release build

```
cmake -G Ninja -B build -DCMAKE_CXX_COMPILER=g++-12 -DCMAKE_BUILD_TYPE=Release .
cmake --build build
env CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test
```

Debug build

```
cmake -G Ninja -B debug -DCMAKE_CXX_COMPILER=g++-12 -DCMAKE_BUILD_TYPE=Debug .
cmake --build debug
env CTEST_OUTPUT_ON_FAILURE=1 cmake --build debug --target test
```

