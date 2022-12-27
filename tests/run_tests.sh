#!/usr/bin/env bash

if [ "${PWD##*/}" != "tests" ]; then
    echo "Error: Please run this script from the tests/ directory"
    exit 1
fi

if ! [ -f "../xterm2ansi" ]; then
    echo "Error: xterm2ansi executable not found"
    exit 1
fi

for infile in *.in; do
    outfile="${infile%.*}.out"
    tempout="${infile%.*}.temp"
    ../xterm2ansi < "$infile" > "$tempout"
    if ! cmp -s "$tempout" "$outfile"; then
        echo "Test failed: $infile"
    else
        echo "Test succeeded: $infile"
    fi
    rm "$tempout"
done

