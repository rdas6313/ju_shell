#!/bin/bash


sum=0
while read value 
do
	sum=$((sum + value))
done <in.txt
echo "Sum is $sum"
