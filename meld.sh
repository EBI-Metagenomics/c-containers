#!/bin/bash

headers="$*"

function die {
    echo "$1" >&2
    exit 1
}

function flip {
    if type tac >/dev/null 2>&1; then
        tac "$@"
    else
        tail -r "$@"
    fi
}
export -f flip

echo "#ifndef CCO_H"
echo "#define CCO_H"
echo

{
    for header in $headers; do
        module=${header%.*}
        first_line=$(head -n 1 "$module.h")
        second_line=$(head -n 2 "$module.h" | tail -n 1)
        last_line=$(flip "$module.h" | head -n 1)

        MODULE=$(echo "$module" | tr '[:lower:]' '[:upper:]')

        if [ "$first_line" != "#ifndef ${MODULE}_H" ]; then die "Improper include guard"; fi
        if [ "$second_line" != "#define ${MODULE}_H" ]; then die "Improper include guard"; fi
        if [ "$last_line" != "#endif" ]; then die "Improper include guard"; fi

        dashes="----------------------------------------------------------------------"
        section=" $module module begin "
        size=${#section}
        echo "/* ____$dashes */" | sed "s/${dashes:0:$size}/$section/" | sed 's/____/----/'

        tail -n +3 "$module.h" |
            flip |
            tail -n +2 |
            flip |
            sed 's/^#include ".*"$//' |
            sed '/./,$!d' |
            flip |
            sed '/./,$!d' |
            flip

        section=" $module module end "
        size=${#section}
        echo "/* ____$dashes */" | sed "s/${dashes:0:$size}/$section/" | sed 's/____/----/'

        echo
    done
} | flip | sed '/./,$!d' | flip

echo
echo "#endif"
