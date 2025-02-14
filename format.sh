#!/bin/zsh

FILE=~/github/dotfiles/clang-format.yml

clang-format -style=file:${FILE} -i src/**/*.hpp src/**/*.h examples/**/*.cpp test/**/*.cpp