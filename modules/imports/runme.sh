#!/bin/bash

CXX=clang++-18

${CXX} --std=c++23 -fmodules --stdlib=libc++ --precompile imported.cppm -o imported.pcm
${CXX} --std=c++23 -fmodules --stdlib=libc++ -fmodule-file=imported=imported.pcm imported.pcm main.cc
