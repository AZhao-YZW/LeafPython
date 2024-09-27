#!/bin/bash

show_help() {
    echo "Usage: $0 [options] <command>"
    echo ""
    echo "Options:"
    echo "  -h, --help          Show this help message and exit."
    echo "  -d, --demo <name>   Specify the build demo."
    echo ""
    echo "Commands:"
    echo "  build              Build demo."
    echo "  clean              Clean demo."
    echo "  run                Run demo."
    echo "  llt                Run LLT."
    echo "  hlt                Run HLT."
    echo "  benchmark          Run benchmark."
}

input_file=""
output_file=""
command=""

while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            exit 0
            ;;
        -d|--demo)
            demo_name="$2"
            demo_path="../demo/$demo_name"
            if [ ! -d $demo_path ]; then
                echo "Demo [$demo_name] not found."
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
        echo "===== Building demo: [$demo_name] ====="
        ./build.sh $demo_path
        ;;
    clean)
        echo "===== Cleaning demo: [$demo_name] ====="
        ./clean.sh $demo_path
        ;;
    run)
        echo "===== Running demo: [$demo_name] ====="
        ./run.sh $demo_path
        ;;
    llt)
        echo "===== Running LLT: [$demo_name] ====="
        ;;
    hlt)
        echo "===== Running HLT: [$demo_name] ====="
        ;;
    benchmark)
        echo "===== Running benchmark: [$demo_name] ====="
        ;;
    *)
        echo "Unknown command: '$command'"
        show_help
        exit 1
        ;;
esac