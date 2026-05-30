#!/usr/bin/bash
binDir='./bin/'
srcDir='./src/'
baseTestName="./test"

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

test_one() {
	local binFile="$binDir$1"
	echo -e "\e[1;32m" --- $1 --- '\e[0m'
	
	echo -e "\e[1;35m" '\t'--- STDLIB --- '\e[0m'
	/usr/bin/time -v $binFile 2>&1 | grep -E "Major|Minor"
	echo -e "\e[1;35m" '\t'--- FT_MALLOC --- '\e[0m'
	echo -n $(export LD_LIBRARY_PATH="$(pwd)/.."; export LD_PRELOAD=libft_malloc.so;		/usr/bin/time -v $binFile)
#		2>&1 | grep -E "Major|Minor")
}

compile_all_test

for fileN in {0..5}; do
	fileName="$baseTestName$fileN"
	test_one "$fileName"
done;
