#!/bin/bash

for item in $(ls $1)
do	
	if [ -w $item ] && [ -x $item ]
	then
		du -h $item
	else
		echo "Write and Execute permission not available for $item"
	fi
	
done
echo "TOTAL: $(du -sh $1)"

