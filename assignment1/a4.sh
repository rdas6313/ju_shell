#!/bin/bash

read -p "Enter value: " value

echo -n "Reverse value: "
while [[ $value -gt 0 ]] ; do
	r=$((value%10))
	value=$((value/10))
	echo -n $r
done
echo


