#!/bin/bash

cd build

../xed/mfile.py --install-dir="../dist/linux" --extra-flags="-fPIC" install

mkdir -p ../../cpp/lib/linux

cp ../dist/linux/lib/* ../../cpp/lib/linux
cp -r ../dist/linux/include/* ../../cpp/include
