#!/bin/bash

for file in $(ls)
do
	if [ -f $file ]
	then
		name=$(echo $file | cut -d "." -f 1)
		extension=$(echo $file | cut -d "." -f 2)
		echo $extension
		if [ $extenstion == "sh" ] -o [ $extenstion == "exe" ]
		then
			echo $name $extension
		fi
		#if [ $1 -eq 0 ]
		#then
		#	mv $name.sh $name.exe
		#else
		#	mv $name.exe $name.sh
		#fi
	fi
done
