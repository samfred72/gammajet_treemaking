#/bin/bash
SPLIT=$1
DATASET=$2
DST=$3
rm queue.list
touch queue.list
for i in `cat runlist.list`; do
  cat /sphenix/user/samfred/projects/queuelists/masterqueue_${DATASET}_${DST}_split${SPLIT}.list | grep $i >> queue.list
done
