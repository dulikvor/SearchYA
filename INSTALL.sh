#!/bin/bash
workingDir=$PWD
export "LIBRARY_PATH=$LIBRARY_PATH:$workingDir/Third_Party/lib/"
export "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$workingDir/Third_Party/lib/"
export "PATH=$PATH:$workingDir/Third_Party/bin/"
export MESOS_WORK_DIR=$workingDir/Third_Party/tmp/mesos-"$USER"
export "CPATH=$CPATH:$workingDir/Third_Party/include/" && cmake . && make