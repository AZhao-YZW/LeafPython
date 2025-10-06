#!/bin/bash

show_help() {
    echo "Usage: $0 <command> [options]"
    echo ""
    echo "command & options:"
    echo "  -h, --help          Show this help message and exit."
    echo "  build               Build project."
    echo "    -t, --type [type]"
    echo "    -c, --case [case]"
    echo "  clean               Clean project."
    echo "    -t, --type [type]"
    echo "    -c, --case [case]"
    echo "  run                 Run project."
    echo "    -t, --type [type]"
    echo "    -c, --case [case]"
}

command=""
type=""
dir=""
path=""

while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            exit 0
            ;;
        -t|--type)
            type="$2"
            case "$type" in
                demo|llt|hlt|benchmark|all_test)
                    ;;
                *)
                    echo "Unknown type: [$type]"
                    exit 1
                    ;;
            esac
            shift
            ;;
        -c|--case)
            if [ "$type" == "" ]; then
                echo "Please specify type before case."
                exit 1
            fi
            dir="$2"
            if [ "$type" == "demo" ]; then
                path="../demo/$dir"
            else
                path="../test/$type/$dir"
            fi
            if [ ! -d $path ]; then
                echo "Path [$path] not found."
                exit 1
            fi
            shift
            ;;
        *)
            command="$1"
            ;;
    esac
    shift
done

case $command in
    build)
        echo "===== Building project: [$dir] ====="
        ./build.sh $type $path
        ;;
    clean)
        echo "===== Cleaning project: [$dir] ====="
        ./clean.sh $type $path
        ;;
    run)
        echo "===== Running project: [$dir] ====="
        ./run.sh $type $path
        ;;
    *)
        echo "Unknown command: '$command'"
        show_help
        exit 1
        ;;
esac