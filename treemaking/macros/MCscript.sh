#!/bin/bash
export USER="$(id -u -n)"
export LOGNAME=${USER}
export HOME=/sphenix/u/${USER}

source /opt/sphenix/core/bin/sphenix_setup.sh -n new 
source /opt/sphenix/core/bin/setup_local.sh /sphenix/user/samfred/projects/gammajet/install/

this_script=$BASH_SOURCE
this_script=`readlink -f $this_script`
this_dir=`dirname $this_script`
echo running: $this_script $*

SIM=$1
LIST=$2
TRIGGER=$3
PROCESS=$4
TEST=0
[ -n "$5" ] && TEST=$5

# go to condor scratch directory
if [[ $TEST == 0 ]]; then
  
  if [[ ! -z "$_CONDOR_SCRATCH_DIR" && -d $_CONDOR_SCRATCH_DIR ]]; then
    cd $_CONDOR_SCRATCH_DIR
  else
    echo condor scratch NOT set
    exit -1
  fi
  
  files=`cat /sphenix/user/samfred/projects/filelists/${SIM}28/${SIM}_${TRIGGER}/${LIST}`
  echo $files
  for file in $files; do
    echo "Copying $file"
    getinputfiles.pl $file
  done
  
  cp /sphenix/user/samfred/projects/gammajet/treemaking/macros/MCFun4All_macro.C .
  cp /sphenix/user/samfred/projects/gammajet/treemaking/macros/G4_CEmc_Spacal_local.C .
fi

echo "input files..."
cat /sphenix/user/samfred/projects/filelists/${SIM}28/${SIM}_${TRIGGER}/${LIST}

root -l -q -b "MCFun4All_macro.C(\"/sphenix/user/samfred/projects/filelists/${SIM}28/${SIM}_${TRIGGER}/${LIST}\",${TEST},\"${TRIGGER}\",\"${SIM}\")"

if [[ $TEST == 0 ]]; then
  cp outtree_$LIST /sphenix/tg/tg01/jets/samfred/gammajet
  rm /sphenix/user/samfred/projects/gammajet/treemaking/macros/MClog/${PROCESS}.out
  rm /sphenix/user/samfred/projects/gammajet/treemaking/macros/MClog/${PROCESS}.err
fi

echo all done
echo "script done"
