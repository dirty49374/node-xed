#!/bin/bash

cd build

../xed/mfile.py --install-dir="../dist/mac" --extra-flags="-fPIC -mmacosx-version-min=10.0" install

mkdir -p ../../cpp/lib/mac

cp ../dist/mac/lib/* ../../cpp/lib/mac

