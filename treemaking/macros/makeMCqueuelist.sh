#/bin/bash
rm MCqueue.list
touch MCqueue.list
while IFS= read -r line || [[ -n "$line" ]]; do
  if [[ $line == "#"* ]]; then
    continue
  fi
  read -r SIM TYPE remaining_text <<< "$line"
  for j in `cat /sphenix/user/samfred/projects/queuelists/masterqueue_${SIM}28_${TYPE}.list`;do
    echo $SIM $j $TYPE >> MCqueue.list
  done
done < "MCrunlist.list"

