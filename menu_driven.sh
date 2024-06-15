
#!/bin/bash
function print(){ 
	# arguemnts $1 -> "String you want print"
	# arguments $2 -> no of times

	clear 
	for i in $(seq 1 $2)
	do
		echo "$i) $1 $i"
	done
}

function is_valid_input(){
	#argument $1 -> input
	#argument $2 -> till which input in valid
	if ! [[ $1 =~ ^[0-9]+$ ]]
	then
		return 0
	fi

	if [ 1 -le $1 ] && [ $1 -le $2 ]
	then
		return 1
	elif [ $1 == $(($2 + 1)) ]
	then
		return -1
	else
		return 0
	fi
}

function call_script(){
	# arguemnt $1 -> selected assignment number
	# argument $2 -> selected question number 
	#./a$1q$2.sh
	cd "assignment$1"
	chmod +x "a$2.sh"
	"./a$2.sh"
	cd ..

}

q_in_a=(5 5 6) #question in assignments
total_assignment=${#q_in_a[@]}

should_run=1

while [ $should_run -eq 1 ]
do
	print "Assignment" $total_assignment
	echo "$((total_assignment+1))) Exit"
	read -p "> " a_op
	is_valid_input "$a_op" $total_assignment
	ret=$?
	if [ $ret -ne 1 ]
	then
		if [ $ret -eq 0 ]
		then
			echo "Invalid input"
		fi
		should_run=0
		continue
	fi

	run_questions=1
	while [ $run_questions -eq 1 ]
	do
		print "Question" ${q_in_a[$((a_op-1))]}
		echo "$((${q_in_a[$((a_op-1))]}+1))) Back"
		read -p "> " q_op
		is_valid_input "$q_op" ${q_in_a[$((a_op-1))]}
		ret=$?
		if [ $ret -ne 1 ]
		then
			if [ $ret -eq 0 ]
			then
				echo "Invalid input"
			else
				run_questions=0
			fi
			continue
		fi
		echo "..........."
		call_script $a_op $q_op
		echo "..........."
		read -s
	done
done


