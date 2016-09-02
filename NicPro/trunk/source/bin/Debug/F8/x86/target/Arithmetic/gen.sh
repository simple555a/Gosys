#! /usr/bin/bash

generateBinary(){
	local tp_ucase op
	
	tp_ucase=`echo $2 | awk '//{print toupper($$0)}'`
	
	case $1 in
	ADD) op="+";;
	SUB) op="-";;
	MUL) op="*";;
	DIV) op="/";;
	AND) op="&";;
	OR) op="|";;
	XOR) op="^";;	
	esac
	
	exec 1>$1_$tp_ucase.f
	
	echo "fblock $1_$tp_ucase;"
	echo "uuid{`f8uuid`};"
	echo var
	echo	"IN1:$2; in;"
	echo "IN2:$2; in;"
	echo "OUT:$2; out;"
	echo implementation
	echo "void main()"
	echo "{"
	echo "OUT = IN1 $op IN2;"
	echo "}"
}

generateUnary(){
	local tp_ucase op
	
	tp_ucase=`echo $2 | awk '//{print toupper($$0)}'`
	
	case $1 in
	NOT) op="!";;
	NEG) op="-";;
	INV) op="~";;
	esac
	
	exec 1>$1_$tp_ucase.f
	
	echo "fblock $1_$tp_ucase;"
	echo "uuid{`f8uuid`};"
	echo var
	echo	"IN:$2; in;"
	echo "OUT:$2; out;"
	echo implementation
	echo "void main()"
	echo "{"
	echo "OUT = $op IN;"
	echo "}"
}

gen_all(){
	for op in ADD SUB MUL DIV; do
		for tp in byte word dword char short integer float double date; do
			generateBinary $op $tp
		done;
	done
}

gen_all2(){
	for op in AND OR XOR; do
		for tp in byte word dword char short integer; do
			generateBinary $op $tp
		done;
	done
}

gen_all3(){
	for op in NOT NEG INV; do
		for tp in byte word dword char short integer; do
#			generateUnary $op $tp
			fcc -i:${op}_${tp}.f
		done;
	done
}

#generate ADD byte
gen_all3