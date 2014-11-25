#!/bin/sh

if [ $# != 1 ]; then
    echo "Usage: export.sh <target_dir>"
    exit
fi

if [ -d $1 ]; then
    echo "Copying remote files to " $1
else
    echo "dir does not exist: " $1
    exit
fi

cp remote/*.h remote/*.cpp remote/*.proto $1
cp os/*.h os/*.cpp $1
cp qml/providers/*.h qml/providers/*.cpp $1
