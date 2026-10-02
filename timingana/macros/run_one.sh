#!/bin/bash
# Run TimingAna over a single DST.
# Usage: ./run_one.sh [DST name] [radius] [nevents (0 = all)] [timecalib 0/1] [outdir]
# Under condor (_CONDOR_SCRATCH_DIR set) the DST is staged into the scratch directory.
source /opt/sphenix/core/bin/sphenix_setup.sh -n new
source /opt/sphenix/core/bin/setup_local.sh /sphenix/user/samfred/projects/gammajet/install/

DST=${1:-DST_JETCALO_run2pp_ana521_2025p007_v001-00047289-00000.root}
R=${2:-0.4}
NEV=${3:-0}
TCAL=${4:-0}
BASE=/sphenix/user/samfred/projects/gammajet/timingana
OUTDIR=${5:-$BASE/output}
mkdir -p $OUTDIR

if [[ -n "$_CONDOR_SCRATCH_DIR" && -d "$_CONDOR_SCRATCH_DIR" ]]; then
  cd $_CONDOR_SCRATCH_DIR
else
  mkdir -p $BASE/output/work && cd $BASE/output/work
fi
[ -f "$DST" ] || getinputfiles.pl $DST

TAG=timing; [[ $TCAL == 1 ]] && TAG=timing_tcal
OUT=$OUTDIR/${TAG}_$(basename $DST .root)_r0${R#0.}.root
TCALB=false; [[ $TCAL == 1 ]] && TCALB=true
root -l -q -b "$BASE/macros/Fun4All_timing.C(\"$DST\",\"$OUT\",$R,$NEV,$TCALB)"
if [[ -n "$_CONDOR_SCRATCH_DIR" ]]; then rm -f $DST; fi
echo "run_one.sh done: $OUT"
