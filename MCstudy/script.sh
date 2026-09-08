#!/bin/bash
export USER="$(id -u -n)"
export LOGNAME=${USER}
export HOME=/sphenix/u/${USER}

hostname

this_script=$BASH_SOURCE
this_script=`readlink -f $this_script`
this_dir=`dirname $this_script`
echo rsyncing from $this_dir
echo running: $this_script $*


source /opt/sphenix/core/bin/sphenix_setup.sh -n new
source /opt/sphenix/core/bin/setup_local.sh /sphenix/user/samfred/projects/gammajet/install/

#printenv 

if [[ -n "$2" ]]; then
  TRIGGER=$1
  SEGMENT=$2
  NAME="${TRIGGER}_${SEGMENT}"
  SMEAR=$3
  echo "sample..."
  echo ${NAME}
  root "histmaker.C(0,1,${SMEAR},\"${NAME}\")"
else
  RUNNUM=$1
  echo "runnumber..."
  echo ${RUNNUM}
  root "histmaker.C(${RUNNUM})"
fi

echo all done
echo "script done"
