#!/bin/bash


for item in $(ls)
do
	if [ -d $item ]
	then
		echo "$item is a Directories"
	elif [ -f $item ]
	then 
		echo "$item is a Regular File"
	fi
done
