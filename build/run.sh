#!/bin/bash
# $1: demo directory
output_dir="$1/output"
demo_name=$(basename "$1")
./$output_dir/"$demo_name"_demo