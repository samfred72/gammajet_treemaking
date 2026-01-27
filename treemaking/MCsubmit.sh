#!/bin/bash

mkdir -p /tmp/samfred
#rm -r /sphenix/tg/tg01/jets/samfred/gammajet_hadded/
#mkdir -p /sphenix/tg/tg01/jets/samfred/gammajet_hadded
condor_submit MCcondor.job
