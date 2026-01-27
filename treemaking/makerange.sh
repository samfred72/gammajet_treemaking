for i in Jet50 Jet70; do
  num=0
  for j in {00..24}; do
    echo $i 0$j[0-9][0-9] $num
    num=$((num+1))
  done
done
#for i in Jet5 Jet10 Jet20 Jet30; do
#  num=0
#  for j in {0..9}; do
#    for k in "[0-4]" "[5-9]"; do
#      echo $i 00$j$k[0-9] $num
#      num=$((num+1))
#    done
#  done
#done
