#!/bin/sh

find . -type f -print   | grep -v build | grep -v git | xargs python cpplint.py  2>&1 | grep -v Done | grep -v Ignoring
