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

cp $1/gfipublisher.h remote/gfipublisher.h
cp $1/gfisubscriber.h remote/gfisubscriber.h
cp $1/gfmetric.h remote/gfmetric.h
cp $1/gfmetric.proto remote/gfmetric.proto
cp $1/gfpublisher.cpp remote/gfpublisher.cpp
cp $1/gfpublisher.h remote/gfpublisher.h
cp $1/gfpublisher.proto remote/gfpublisher.proto
cp $1/gfpublisher_skel.cpp remote/gfpublisher_skel.cpp
cp $1/gfpublisher_skel.h remote/gfpublisher_skel.h
cp $1/gfsubscriber.proto remote/gfsubscriber.proto
cp $1/gfsubscriber_stub.cpp remote/gfsubscriber_stub.cpp
cp $1/gfsubscriber_stub.h remote/gfsubscriber_stub.h

cp $1/gfsocket.cpp os/gfsocket.cpp
cp $1/gfsocket.h os/gfsocket.h
cp $1/gfthread.cpp os/gfthread.cpp
cp $1/gfthread.h os/gfthread.h

cp $1/gfcpu_provider.cpp qml/providers/gfcpu_provider.cpp
cp $1/gfcpu_provider.h qml/providers/gfcpu_provider.h
cp $1/gfprovider.cpp qml/providers/gfprovider.cpp
cp $1/gfprovider.h qml/providers/gfprovider.h
