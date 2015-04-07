#!/bin/bash
./generate_changelog.sh > ChangeLog
cmake CMakeLists.txt
make
