#!/usr/bin/bash
: '
for n in {1..10}
do echo "$n"
done

#array
s=("san" "cha" "gee")
for i in ${s[@]}
do
echo "$i"
done


for ((i=0; i<10; i++))
do
echo $i
done
'
#while loop

a=7
while [ "$a" -ge 4 ];
do
echo $a
a=$((a-1))
done
