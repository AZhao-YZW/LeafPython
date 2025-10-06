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
    rm -rf "$output_dir"
else
    cd $path/build || exit 1
    ./clean.sh
fi