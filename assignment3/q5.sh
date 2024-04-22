#!/bin/bash

for file in $(ls)
do
	if [ -f $file ]
	then
		name=$(echo $file | cut -d "." -f 1)
		extension=$(echo $file | cut -d "." -f 2)
		if [ "./$name.$extension" == "$0" ] || [ $extension != "sh" ] && [ "$extension" != "exe" ]
		then
			continue
		fi
		if [ "$1" == "0" ]
		then
			mv $name.sh $name.exe
			if [ $? -eq 0 ]
			then
				echo "Converted files from $name.sh to $name.exe"
			fi
		else
			mv $name.exe $name.sh
			if [ $? -eq 0 ]
			then
				echo "Converted files from $name.exe to $name.sh"
			fi
			
		fi
	fi
done
