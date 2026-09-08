#!/bin/bash

rm log/*
mkdir -p /tmp/samfred
condor_submit condor.job

sleep 5
nruns=`cat runlist.list | wc -l`
top=`expr $nruns - 1`
for run in $( seq 0 $top ); do
  while ! grep -q "script done" "log/${run}.out" 2>&1; do
    echo "Waiting on $run..." 
    sleep 5
  done
  echo "Run $run complete"
  if [[ $run == 1567 ]]; then
    sleep 1
    bash dohadd.sh &
  fi
done
sleep 1
bash doMChadd.sh
echo "Now waiting for data..."
wait
