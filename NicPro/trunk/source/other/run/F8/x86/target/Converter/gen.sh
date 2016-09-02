#! /usr/bin/bash

shorttype(){
	local shortname
	case $1 in
	byte ) shortname="B";;
	word ) shortname="W";;
	dword ) shortname="DW";;
	char ) shortname="C";;
	short ) shortname="S";;
	integer ) shortname="I";;
	float ) shortname="F";;
	double ) shortname="DBL";;
	date) shortname="DT";;
	esac
	echo $shortname
}

generate(){
	local t1ucase t2ucase
	
	if [ "$1" != "$2" ]; then
#		echo $1 to $2 ...
		
		t1ucase=`shorttype $1`
		t2ucase=`shorttype $2`
		
		exec 1>${t1ucase}2$t2ucase.f
		
		echo "fblock ${t1ucase}2${t2ucase};"
		echo "uuid{`f8uuid`};"
		echo var
		echo	"IN:$1; in;"
		echo "OUT:$2; out;"
		echo implementation
		echo "void main()"
		echo "{"
		echo "OUT = (${2})IN;"
		echo "}"
	fi
}

gen_all(){
	for t1 in byte word dword char short integer float double date; do
		for t2 in byte word dword char short integer float double date; do
			generate $t1 $t2
#			echo $t1 ... $t2
		done;
	done
}

#generate byte integer
gen_all
