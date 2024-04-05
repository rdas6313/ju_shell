#!/bin/bash

read -p "Enter value: " value

sum=0
while [[ $value -gt 0 ]] ; do
	r=$((value%10))
	value=$((value/10))
	sum=$((sum+r))
done

echo "Sum: $sum"
