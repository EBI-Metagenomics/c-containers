#!/bin/bash

headers="$*"

function die {
    echo "$1" >&2
    exit 1
}

echo "#ifndef CCO_H"
echo "#define CCO_H"
echo

{
    for header in $headers; do
        module=${header%.*}
        first_line=$(head -n 1 "$module.h")
        second_line=$(head -n 2 "$module.h" | tail -n 1)
        last_line=$(tail -r "$module.h" | head -n 1)

        MODULE=$(echo "$module" | tr '[:lower:]' '[:upper:]')

        if [ "$first_line" != "#ifndef ${MODULE}_H" ]; then die "Improper include guard"; fi
        if [ "$second_line" != "#define ${MODULE}_H" ]; then die "Improper include guard"; fi
        if [ "$last_line" != "#endif" ]; then die "Improper include guard"; fi

        dashes="----------------------------------------------------------------------"
        section=" $module module begin "
        size=${#section}
        echo "/* ____$dashes */" | sed "s/${dashes:0:$size}/$section/" | sed 's/____/----/'

        tail -n +3 "$module.h" |
            tail -r |
            tail -n +2 |
            tail -r |
            sed 's/^#include ".*"$//' |
            sed '/./,$!d' |
            tail -r |
            sed '/./,$!d' |
            tail -r

        section=" $module module end "
        size=${#section}
        echo "/* ____$dashes */" | sed "s/${dashes:0:$size}/$section/" | sed 's/____/----/'

        echo
    done
} | tail -r | sed '/./,$!d' | tail -r

echo
echo "#endif"
