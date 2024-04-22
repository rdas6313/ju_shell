#!/bin/bash

count=0
for file in $(ls)
do
	if [[ -f $file  &&  $file == **.sh* ]]
	then
		((count+=1))
	fi
done
echo "Total .sh files: $count"
