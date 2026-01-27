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

TRIGGER=$1
SEGMENT=$2
NAME="${TRIGGER}_${SEGMENT}"
SMEAR=$3

source /opt/sphenix/core/bin/sphenix_setup.sh -n new
source /opt/sphenix/core/bin/setup_local.sh /sphenix/user/samfred/projects/gammajet/install/

#printenv 

echo "runnumber..."
echo ${NAME}
root "histmaker.C(0,1,${SMEAR},\"${NAME}\")"

echo all done
echo "script done"
