#!/bin/bash

git init .
git submodule add https://github.com/libopencm3/libopencm3.git
git commit -m "Import OpenCM3"

pushd libopencm3
unset CFLAGS && unset CXXFLAGS
make
popd
