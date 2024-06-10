#!/bin/bash

##
# Run CodeChecker
##


# set error handling
set -eu -o pipefail

# working directory
WORKING_DIR=$(pwd)


#
# default values
#

# CodeChecker command
CODECHECKER_COMMAND=${CODECHECKER_COMMAND:-$(command -v "CodeChecker" || echo "")}

# number of parallel jobs
PARALLEL=${PARALLEL:-"1"}

# code checker log file (compile_commands.json)
CODECHECKER_LOGFILE=${CODECHECKER_LOGFILE:-""}

# code checker working directory
CODECHECKER_WORKING_DIR=${CODECHECKER_WORKING_DIR:-"$WORKING_DIR/tmp/codechecker"}

# HTML output directory
CODECHECKER_HTML_OUTPUT_DIR=${CODECHECKER_HTML_OUTPUT_DIR:-"$CODECHECKER_WORKING_DIR/html-output"}

# source code root directory
SOURCE_CODE_ROOT_DIR=${SOURCE_CODE_ROOT_DIR:-"$WORKING_DIR/lux_sp"}

# list of source code files to analyze
# note: codechecker should analyse cpp files, not headers.
SOURCE_CODE_FILES=${SOURCE_CODE_FILES:-$(find "$SOURCE_CODE_ROOT_DIR" -type f -regex ".*\(c\|cc\|cpp\|cxx\)$")}


#
# command line options
#

PRINT_AND_EXIT=0
while getopts "hp" opt; do
    case ${opt} in
    h)
        echo "Usage: $0 [-h] [-p] [OPTION...]"
        echo "  -h: print this help text and exit"
        echo "  -p: print definitions and exit"
        exit 0
        ;;
    p)
        PRINT_AND_EXIT=1
        ;;
    \?)
        echo "Unknown options found"
        exit 1
        ;;
    esac
done
shift "$((OPTIND - 1))"


#
# prepare context
#

# print definitions
echo "CODECHECKER_COMMAND: [$CODECHECKER_COMMAND]"
echo "PARALLEL: [$PARALLEL]"
echo "CODECHECKER_LOGFILE: [$CODECHECKER_LOGFILE]"
echo "CODECHECKER_WORKING_DIR: [$CODECHECKER_WORKING_DIR]"
echo "CODECHECKER_HTML_OUTPUT_DIR: [$CODECHECKER_HTML_OUTPUT_DIR]"
echo "SOURCE_CODE_ROOT_DIR: [$SOURCE_CODE_ROOT_DIR]"
echo "SOURCE_CODE_FILES: [$SOURCE_CODE_FILES]"
# exit if print-and-exit option is set
if [ "$PRINT_AND_EXIT" -ne 0 ]; then
    exit 0
fi


#
# sanity checks
#

# the code checker command must be present
if [ -z "$CODECHECKER_COMMAND" ]; then
    echo "This tool requires CodeChecker. Install it please, and then run this tool again."
    exit 1
fi

# code checker log file must be set
if [ -z "$CODECHECKER_LOGFILE" ]; then
    echo "CODECHECKER_LOGFILE not set"
    exit 1
fi


#
# code checker operations
#

# set shell to print trace
set -x

# create code checker working directory
mkdir -p "$CODECHECKER_WORKING_DIR"

# provisional files
analysis_output_dir="$CODECHECKER_WORKING_DIR/analysis-output"
skip_file="$CODECHECKER_WORKING_DIR/skip-file.txt"
tidyargs_file="$CODECHECKER_WORKING_DIR/tidyargs-file.txt"
cppcheckargs_file="$CODECHECKER_WORKING_DIR/cppcheckargs-file.txt"

# create skip file
cat > "$skip_file" <<EOF
-/usr/*
-*/_deps/fmt-src/*
EOF

# create tidy-args file
cat >"$tidyargs_file" <<EOF
-header-filter=.*
EOF

# create cppcheck-args file
cat >"$cppcheckargs_file" <<EOF
--check-level=exhaustive
--inline-suppr
EOF

# convert string of source code files to array
readarray -t file_list <<<"$SOURCE_CODE_FILES"

# code analysis
rm -rf "$analysis_output_dir"
"$CODECHECKER_COMMAND" analyze \
    --jobs "$PARALLEL" \
    --file "${file_list[@]}" \
    --ignore "$skip_file" \
    --analyzers clangsa clang-tidy cppcheck \
    --analyzer-config \
    clang-tidy:take-config-from-directory=true \
    cppcheck:libraries=googletest.cfg \
    --cppcheckargs "$cppcheckargs_file" \
    --tidyargs "$tidyargs_file" \
    --output "$analysis_output_dir" \
    "$CODECHECKER_LOGFILE"

# generate HTML report
# mkdir -p "$(dirname "$CODECHECKER_HTML_OUTPUT_DIR")"
rm -rf "$CODECHECKER_HTML_OUTPUT_DIR"
"$CODECHECKER_COMMAND" parse \
    --trim-path-prefix "$WORKING_DIR" \
    --export html \
    --output "$CODECHECKER_HTML_OUTPUT_DIR" \
    "$analysis_output_dir"
    # > /dev/null || echo "exit code $? ignored"
