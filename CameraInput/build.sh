#!/bin/bash
cmake CMakeLists.txt
make
sudo make install
cp ./bin/generic_executabl* ./output