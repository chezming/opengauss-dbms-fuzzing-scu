#!/bin/bash
cd $TMP_DIR/dot-files


function larger_mv()
{ 
    while read file; do
    {
        echo "$file"
        awk '!a[$0]++' "$file" > "$file".smaller.dot
        mv "$file".smaller.dot "$file"
        sed -i 's/\\\\\"//g' "$file"
        sed -i 's/\[.\"]//g' "$file"
        sed -i 's/\(^\s*[0-9a-zA-Z_]*\):[a-zA-Z0-9]*\( -> \)/\1\2/g' "$file"
    }&
    done
    wait
}

export -f larger_mv

find $TMP_DIR/dot-files/ -type f -name "cfg.*.dot" | larger_mv
