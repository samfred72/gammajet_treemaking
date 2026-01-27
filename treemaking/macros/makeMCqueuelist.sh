#/bin/bash
rm queue.list
touch queue.list
for i in `cat MCrunlist.list`; do
  for j in `cat /sphenix/user/samfred/projects/queuelists/masterqueue_pythia28_$i.list`;do
    echo $j $i >> queue.list
  done
done
