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

exec_grep() {
	#$@
	local res=$(($@ 1>/dev/null) 2>&1 | grep -E "Major|Minor")
	echo $res | tr -cd '[:digit:] ' | awk '{printf("\t\tMajor: \033[34m%d\033[0m\n\t\tMinor: \033[34m%d\033[0m\n", $1, $2)}'
}

test_one() {
	local binFile="$binDir$1"
	
	echo -e "\e[1;32m" --- $1 --- '\e[0m'
	echo -e "\e[1;35m" '\t'--- STDLIB --- '\e[0m'

	exec_grep	"/usr/bin/time -v $binFile"

	echo -e "\e[1;35m" '\t'--- FT_MALLOC --- '\e[0m'
	
	exec_grep "$runSh /usr/bin/time -v $binFile"
}

compile_all_test

#test_one ./test0
for fileN in {0..2}; do
	fileName="$baseTestName$fileN"
	test_one "$fileName"
done;
for fileN in {3..5}; do
	fileName="$binDir$baseTestName$fileN"
	echo -e "\e[1;32m" --- $baseTestName$fileN --- '\e[0m'
	$runSh $fileName
done;

