#!/bin/bash

clang-format -style=file -i Main.cpp
find src -type f -name "*" -exec clang-format -style=file -i {} \;
find include -type f -name "*" -exec clang-format -style=file -i {} \;
