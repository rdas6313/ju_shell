#!/bin/bash

IFS=':' read -ra lines <<< "$PATH"

for line in ${lines[@]}
do
	echo $line
	if [ -e $line ]
	then
		ls -ld $line | cut -d " " -f 1
		date -r $line "+%m-%d-%Y %H:%M:%S"

	fi
done
