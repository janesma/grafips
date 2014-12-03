#!/bin/sh

if [ $# != 1 ]; then
    echo "Usage: import.sh <src_dir>"
    exit
fi

if [ -d $1 ]; then
    echo "Copying remote files from " $1
else
    echo "dir does not exist: " $1
    exit
fi

cp $1/remote/*.h remote
cp $1/os/*.h os
cp $1/sources/*.h sources
cp $1/remote/*.cpp remote
cp $1/os/*.cpp os
cp $1/sources/*.cpp sources

cp $1/remote/*.proto remote
