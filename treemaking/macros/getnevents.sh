events=0
while read -r file _; do
  [ -z "$file" ] && continue
  e=`psql FileCatalog -c "select events from datasets where filename='$file';" | head -n 3 | tail -n 1`
  events=$((e+events))
done < "$1"
echo $events
