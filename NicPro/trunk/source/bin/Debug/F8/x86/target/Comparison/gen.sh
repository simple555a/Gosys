#! /usr/bin/bash

generate(){
	local tp_ucase op
	
	tp_ucase=`echo $2 | awk '//{print toupper($$0)}'`
	
	case $1 in
	GT) op=">";;
	LT) op="<";;
	EQ) op="==";;
	NE) op="!=";;
	GE) op=">=";;
	LE) op="<=";;
	esac
	
	exec 1>$1_$tp_ucase.f
	
	echo "fblock $1_$tp_ucase;"
	echo "uuid{`f8uuid`};"
	echo var
	echo	"IN1:$2; in;"
	echo "IN2:$2; in;"
	echo "OUT:bool; out;"
	echo implementation
	echo "void main()"
	echo "{"
	echo "OUT = IN1 $op IN2? 1 : 0;"
	echo "}"
}

gen_all(){
	for op in GT LT EQ NE GE LE; do
		for tp in byte word dword char short integer float double date; do
#		for tp in integer; do
			generate $op $tp
		done;
	done
}

#generate GT byte
gen_all