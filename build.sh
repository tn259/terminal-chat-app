#!/bin/bash

if [[ -f "$(pwd)/build" ]]; then
	rm -rf build
fi
mkdir -p build
cd build
cmake ../
make
cd -
