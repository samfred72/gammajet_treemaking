files=`cat $1`
events=0
for file in $files; do
  e=`psql FileCatalog -c "select events from datasets where filename='$file';" | head -n 3 | tail -n 1`
  events=$((e+events))
done
echo $events
