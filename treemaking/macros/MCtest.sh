#!/bin/bash
file=`head -n 1 MCqueue.list`
#file=`head -n 4993 MCqueue.list | tail -n 1`
bash MCscript.sh $file 0 1
