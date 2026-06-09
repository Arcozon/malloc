#!/usr/bin/bash
binDir='./bin/'
srcDir='./src/'
baseTestName="./test"
runSh=$binDir"run_linux.sh"

compile_all_test() {

	mkdir -p "$binDir"
	for fileN in {0..2}; do
		local fileName="$baseTestName$fileN"
		gcc -o "$binDir$fileName" "$srcDir$fileName".c
	done;
	for fileN in {3..5}; do
		local fileName="$baseTestName$fileN"
		gcc -o "$binDir$fileName" "$srcDir$fileName".c -L.. -lft_malloc
	done;
}

echo_grep() {
	local res=$(($@ 1>/dev/null) 2>&1 | grep -E "Major|Minor")
	echo $res | tr -cd '[:digit:] '
}

test_one() {
	local binFile="$binDir$1"
	
	echo -e "\n\e[1;32m" --- $1 --- '\e[0m'
	echo -e "\e[1;35m" '\t'--- STDLIB --- '\e[0m'

	echo_grep "/usr/bin/time -v $binFile" | awk '{printf("\t\tMajor (requiring I/O) page faults: \033[34m%8d\033[0m\n\t\tMinor (reclaiming a frame) page faults: \033[34m%3d\033[0m\n", $1, $2)}' 

	echo -e "\e[1;35m" '\t'--- FT_MALLOC --- '\e[0m'
	
	echo_grep "$runSh /usr/bin/time -v $binFile" | awk '{printf("\t\tMajor (requiring I/O) page faults: \033[34m%8d\033[0m\n\t\tMinor (reclaiming a frame) page faults: \033[34m%3d\033[0m\n", $1, $2)}' 
}

test_n0() {
	echo -e "\n\e[1;32m" --- Allocating 1 MegaByte --- '\e[0m'
	local max0=$(($("$runSh" /usr/bin/time -v "$binDir$baseTestName"0) 1>/dev/null) 2>&1 | grep -E "Maximum" | tr -cd '[:digit:] ')
	local max1=$(($("$runSh" /usr/bin/time -v "$binDir$baseTestName"1) 1>/dev/null) 2>&1 | grep -E "Maximum" | tr -cd '[:digit:] ')
	local max=$(bc<<<"$max1-$max0")
	local minor0=$(($("$runSh" /usr/bin/time -v "$binDir$baseTestName"0) 1>/dev/null) 2>&1 | grep -E "Minor" | tr -cd '[:digit:] ')
	local minor1=$(($("$runSh" /usr/bin/time -v "$binDir$baseTestName"1) 1>/dev/null) 2>&1 | grep -E "Minor" | tr -cd '[:digit:] ')
	local minor=$(bc<<<"$minor1-$minor0")
	echo $max | awk '{printf("\tMaximum resident set size (kbytes): \033[34m%d\033[0m Kb (\033[34m%.3f\033[0m Mb)\n", $1, $1 / 1024.0f)}' 
	local mult=$(bc <<<"$minor"'*'$(getconf PAGESIZE))
	echo $minor $(getconf PAGESIZE) $mult | awk '{printf("\tMinor * PageSize: %d * %d = \033[34m%8d\033[0m b  (\033[34m%.3f\033[0m Mb)\n", $1, $2, $3, $3 / 1048576.0f)}' 
}
test_n1() {
	echo -e "\n\e[1;32m" --- Page used compared to real malloc '(test1.c)' --- '\e[0m'
	local minorStd=$(($(/usr/bin/time -v "$binDir$baseTestName"1) 1>/dev/null) 2>&1 | grep -E "Minor" | tr -cd '[:digit:] ')
	local minorLibFt=$(($("$runSh" /usr/bin/time -v "$binDir$baseTestName"1) 1>/dev/null) 2>&1 | grep -E "Minor" | tr -cd '[:digit:] ')
	local minorDiff=$(bc<<<"$minorLibFt-$minorStd")
	echo $minorStd | awk '{printf("\tStdLib Minor page faults: \033[34m%6d\033[0m\n", $1)}' 
	echo $minorLibFt | awk '{printf("\tFt_Malloc Minor page faults: \033[34m%3d\033[0m\n", $1)}' 
	echo $minorDiff | awk '{printf("\tDiff: \033[34m%8d\033[0m\n", $1)}' 
}
test_n2() {
	echo -e "\n\e[1;32m" --- Page used when freeing --- '\e[0m'
	local minor0=$(($("$runSh" /usr/bin/time -v "$binDir$baseTestName"0) 1>/dev/null) 2>&1 | grep -E "Minor" | tr -cd '[:digit:] ')
	local minor1=$(($("$runSh" /usr/bin/time -v "$binDir$baseTestName"1) 1>/dev/null) 2>&1 | grep -E "Minor" | tr -cd '[:digit:] ')
	local minor2=$(($("$runSh" /usr/bin/time -v "$binDir$baseTestName"2) 1>/dev/null) 2>&1 | grep -E "Minor" | tr -cd '[:digit:] ')
	echo $minor0 | awk '{printf("\ttest0 (without malloc) Minor page faults: \033[34m%4d\033[0m\n", $1)}' 
	echo $minor1 | awk '{printf("\ttest1 (without free)   Minor page faults: \033[34m%4d\033[0m\n", $1)}' 
	echo $minor2 | awk '{printf("\ttest2 (with free)      Minor page faults: \033[34m%4d\033[0m\n", $1)}' 
}
compile_all_test

for fileN in {0..2}; do
	fileName="$baseTestName$fileN"
	test_one "$fileName"
done;
test_n0
test_n1
test_n2
for fileN in {3..5}; do
	fileName="$binDir$baseTestName$fileN"
	echo -e "\n\e[1;32m" --- $baseTestName$fileN --- '\e[0m'
	$runSh $fileName
done;

