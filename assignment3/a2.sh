#!/bin/bash

total=0
for item in $(ls)
do
	if [ -f $item ]
	then
		echo "File:  $item "
	elif [ -d $item ]
	then 
		echo "Dir: $item"
	fi
	total=$(( total + 1))
done
echo "Total files and directories: $total"
