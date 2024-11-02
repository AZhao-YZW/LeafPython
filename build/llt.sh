#!/bin/bash
# $1: llt_path
# $2: all, if specified, run all tests in the llt_path
if [ "$2" == "all" ]; then
    cd $1 || exit 1
    cur_dir=$(pwd)
    lib_test_dir="lib_test"
    testcore_test_dir="core_test/testcore_test"
    cd $lib_test_dir/build || exit 1
    ./build_and_run.sh
    cd $cur_dir || exit 1
    cd $testcore_test_dir/build || exit 1
    ./build_and_run.sh
else
    cd $1/build || exit 1
    ./build_and_run.sh
fi