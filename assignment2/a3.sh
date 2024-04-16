

read -ra lines <<< $(cat /proc/cpuinfo)

echo "${lines[3]} ${lines[4]} ${lines[5]}"
echo "${lines[6]} ${lines[7]} ${lines[8]} ${lines[9]}"
echo "${lines[13]} ${lines[14]} ${lines[15]} ${lines[16]} ${lines[17]} ${lines[18]} ${lines[19]} ${lines[20]} ${lines[21]}"
echo "${lines[28]} ${lines[29]} ${lines[30]} ${lines[31]}"
echo "${lines[32]} ${lines[33]} ${lines[34]} ${lines[35]} ${lines[36]} "
  
