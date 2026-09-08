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
SIM=$2
NAME=${TRIGGER}_${SIM}

source /opt/sphenix/core/bin/sphenix_setup.sh -n new
source /opt/sphenix/core/bin/setup_local.sh /sphenix/user/samfred/projects/gammajet/install/

printenv 

echo "name..."
echo $NAME
hadd -f -k /sphenix/tg/tg01/jets/samfred/gammajet_full_hadded/gammajet_${SIM}_${TRIGGER}.root /sphenix/tg/tg01/jets/samfred/gammajet_MC/*${SIM}_${TRIGGER}.root

echo all done
echo "script done"
