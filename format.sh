#!/bin/zsh

clang-format -style=file:../dotfiles/clang-format.yml -i src/**/*.hpp src/**/*.h examples/**/*.cpp test/**/*.cpp