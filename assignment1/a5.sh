#!/bin/bash

read -p "Enter login name: " name
data=`cat /etc/passwd`
len=${#name}
name+=":"
len=$((len+1))
array=("username" "password" "userid" "groupid" "userid_info" "home_dir" "login_shell")
for x in $data
do
	user=${x:0:len}
	if [ "$user" == "$name" ]; then
		for i in {1..7}
		do 
			j=$(($i-1))
			echo -n "${array[$j]} : "
			sub=$(echo $x | cut -d ":" -f $i)
			echo $sub
		done
	fi
	
done
