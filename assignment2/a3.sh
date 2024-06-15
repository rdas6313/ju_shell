IFS=$'\n'
lines=( $(cat /proc/cpuinfo) )
echo "${lines[1]}"
echo "${lines[4]}"
echo "${lines[7]}"
echo "${lines[8]}"