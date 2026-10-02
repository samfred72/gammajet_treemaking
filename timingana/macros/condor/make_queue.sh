#!/bin/bash
# Build queue.list for condor from Dading Chen's time-calibrated DST_CALOFITTING list:
# NRUNS runs spread evenly over his 284-run calibrated list, first NSEG segments of each.
# Usage: ./make_queue.sh [NRUNS=20] [NSEG=1]
NRUNS=${1:-20}
NSEG=${2:-1}
D=/sphenix/user/dading/DST_CALO_JETS_JET10GEV/DST_CALOFITTING_pro001/pass_ihcal_statistics_4000
RUNS=$D/ppRun2024_MB_triggered_ihcal_time_stat_4000_cuts.txt
DSTS=$D/ppRun2024_JET_MB_triggered_all_ihcal_time_stat_4000_cut_calolist.list
NTOT=$(wc -l < $RUNS)
STRIDE=$(( NTOT / NRUNS )); (( STRIDE < 1 )) && STRIDE=1
> queue.list
for run in $(awk -v s=$STRIDE -v n=$NRUNS '(NR-1)%s==0 && ++k<=n' $RUNS); do
  grep -- "-$(printf %08d $run)-" $DSTS | sort | head -n $NSEG >> queue.list
done
echo "$(wc -l < queue.list) DSTs from $NRUNS runs (every ${STRIDE}th of $NTOT) written to queue.list"
