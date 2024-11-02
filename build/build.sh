#!/bin/bash
# $1: demo directory
output_dir="$1/output"
demo_name=$(basename "$1")
mkdir -p "$output_dir"
cd "$output_dir" || exit 1
cmake -G "Unix Makefiles" -D demo_name="$demo_name" ..
make -j