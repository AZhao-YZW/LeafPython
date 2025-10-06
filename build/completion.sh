#!/bin/bash

TYPE_OPTIONS="demo llt hlt benchmark all_test"
DEMO_CASE_OPTIONS="debug file_mode repl_mode"
LLT_CASE_OPTIONS="core_test/leafpy_test core_test/testcore_test lib_test"
HLT_CASE_OPTIONS=""
BENCHMARK_CASE_OPTIONS=""

# 补全函数
_leafpy_completion() {
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"

    # 检查是否已经设置了主命令(build/clean/run)
    local main_command_set=0
    local main_command=""
    local type_set=0
    local selected_type=""
    
    local i
    for ((i=1; i<COMP_CWORD; i++)); do
        case "${COMP_WORDS[i]}" in
            build|clean|run)
                main_command_set=1
                main_command="${COMP_WORDS[i]}"
                ;;
            -t|--type)
                type_set=1
                # 获取type的值（下一个参数）
                if [[ $((i+1)) -lt $COMP_CWORD ]]; then
                    selected_type="${COMP_WORDS[$((i+1))]}"
                fi
                ;;
        esac
    done

    case "${prev}" in
        build|clean|run)
            # 在主命令后仅提供-type选项
            COMPREPLY=( $(compgen -W "-t --type" -- "${cur}") )
            return 0
            ;;
        -t|--type)
            # 在-t参数后提供type选项
            COMPREPLY=( $(compgen -W "${TYPE_OPTIONS[*]}" -- "${cur}") )
            return 0
            ;;
        -c|--case)
            # 根据不同的type提供不同的case补全
            case "${selected_type}" in
                demo)
                    COMPREPLY=( $(compgen -W "${DEMO_CASE_OPTIONS[*]}" -- "${cur}") )
                    ;;
                llt)
                    COMPREPLY=( $(compgen -W "${LLT_CASE_OPTIONS[*]}" -- "${cur}") )
                    ;;
                hlt)
                    COMPREPLY=( $(compgen -W "${HLT_CASE_OPTIONS[*]}" -- "${cur}") )
                    ;;
                benchmark)
                    COMPREPLY=( $(compgen -W "${BENCHMARK_CASE_OPTIONS[*]}" -- "${cur}") )
                    ;;
                *)
                    # 默认情况或未识别的type
                    COMPREPLY=()
                    ;;
            esac
            return 0
            ;;
    esac

    # 根据不同状态提供不同的选项
    if [[ $main_command_set -eq 0 ]]; then
        # 未设置主命令时，仅提供主命令选项
        opts="-h --help build clean run"
    elif [[ $type_set -eq 0 ]]; then
        # 已设置主命令但未设置-type时，仅提供-type选项
        opts="-t --type"
    elif [[ -n "${selected_type}" ]]; then
        # 已设置-type且有具体值时，仅提供-case选项
        opts="-c --case"
    else
        # 其他情况提供空选项
        opts=""
    fi

    # 补全命令和选项
    if [[ -n "${opts}" ]]; then
        COMPREPLY=( $(compgen -W "${opts}" -- "${cur}") )
    fi
    
    return 0
}

# 注册补全函数
complete -F _leafpy_completion leafpy_test.sh