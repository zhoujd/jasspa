#!/bin/bash

PLATFORM=`uname`
if [ $PLATFORM = "Linux" ] ; then
    MAKEFILE=linuxgcc-static.mak
else
    echo "Build static on $PLATFORM is not support!"
    exit 1
fi

if [ -n "$MAKEFILE" ] ; then
    make -f $MAKEFILE
fi
