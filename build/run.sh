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
    ./$output_dir/"$proj_name"
else
    cd $path/build || exit 1
    ./run.sh
fi