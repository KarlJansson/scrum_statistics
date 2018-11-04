#!/bin/bash

if ! [ -x "$(command -v cmake)" ]; then
  echo 'Error: cmake is not installed.' >&2
  exit 1
fi

if ! [ -x "$(command -v clang++)" ]; then
  echo 'Error: clang is not installed.' >&2
  exit 1
fi

if [ -x "$(command -v cmakemaker)" ]; then
  cmakemaker
fi

rm -rf ./_build_release_
mkdir _build_release_
cd _build_release_
CXX=clang++ cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_USER_MAKE_RULES_OVERRIDE=~/ClangOverrides.txt -DCMAKE_EXPORT_COMPILE_COMMANDS=YES
cp compile_commands.json ..
cd ..

rm -rf ./_build_debug_
mkdir _build_debug_
cd _build_debug_
CXX=clang++ cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_USER_MAKE_RULES_OVERRIDE=~/ClangOverrides.txt -DCMAKE_EXPORT_COMPILE_COMMANDS=YES
