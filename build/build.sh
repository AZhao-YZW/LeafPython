#!/bin/bash

set -e
if [ $# -ne 2 ]; then
    echo "Usage: <type> <path>"
    exit 1
fi
type=$1
path=$2

if [ "$type" == "demo" ]; then
    output_dir="$path/output"
    proj_name=$(basename "$path")
    mkdir -p "$output_dir"
    cd "$output_dir" || exit 1
    cmake -G "Unix Makefiles" -D proj_name="$proj_name" ..
    make -j
else
    cd $path/build || exit 1
    ./build.sh
fi