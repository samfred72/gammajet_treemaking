#!/bin/bash
# Submit the TimingAna condor jobs listed in queue.list.
mkdir -p /tmp/samfred log /sphenix/tg/tg01/jets/samfred/timingana
condor_submit condor.job
