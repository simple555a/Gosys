#pragma warning(disable:4996)  // Zander 2013-1-22

#include "precomp.h"

CArgsExA::CArgsExA()
{
	argc=0;
	argv=0;
	arg_idx=0;
	args=0;
	len=0;
	arg_len=0;
}

CArgsExA::CArgsExA(
	const char * src, 
	const char * delimiter, 
	int arg_len,
	int maxlen,
	bool ignore_empty_fields
){
	parse(src, delimiter, arg_len, maxlen, ignore_empty_fields);
}

CArgsExA::~CArgsExA()
{
	destroy();
}

void CArgsExA::destroy()
{
	if(args){
		free(args);
		args = 0;
	}
	if(arg_idx){
		delete arg_idx;
		arg_idx = 0;
	}
	if(argv){
		delete argv;
		argv = 0;
	}
	argc = 0;
	len = 0;
	arg_len=0;
}

bool CArgsExA::create(int max_argc, int maxlen)
{
	args = new char[maxlen];
	arg_idx = new int[max_argc];
	argv = new char *[max_argc];
	if(!arg_idx || !argv || !args){
		destroy();
		return false;
	}
	len = maxlen;
	arg_len = max_argc;
	return true;
}

//把src的内容赋给args、argv、arg_idx、argc
int CArgsExA::parse(
	const char * src, 
	const char * delimiter,
	int max_argc,
	int maxlen,
	bool ignore_empty_fields
){
	if(!max_argc){
		max_argc = MAX_ARGC;
	}
	if(!maxlen){
		maxlen = MAX_LINE_LENGTH;
	}
	if(!delimiter) {
		delimiter=" \n";
	}

	// if buffer is reusable?
	if(arg_len < max_argc || maxlen < len){
		destroy();
		if( !create(max_argc, maxlen) ){
			return 0;
		}
	}	

	strncpy(args, src, len);
	args[len - 1] = '\0';

	int i;
	char in_quotation;
	char * p,*marker;	 

	for(i=0;i<max_argc;i++) argv[i]="";
	argc=0;
	
	p=marker=args;
	in_quotation=false;
	while(*p){
		if(in_quotation){
			// in quatation mode, all demiliters are ignored
			// until a matching '"' is found
			if('"' == *p){
				in_quotation = FALSE;
				if(*marker == '"' && (strchr(delimiter,p[1]) || !p[1])){
					/* remove leading and trailing ' " '  for this piece if
					this piece is surrounded by a pair of quote signs,
					note that if the quote signs are in middle of the
					string, then they are not removed.
					*/
					marker++;
					*p = 0;
				}
			}
		}else if('"' == *p){
			in_quotation = TRUE;
		}else if(strchr(delimiter,*p)){
			// got an argument, emit it to the argument buffer
			*p=0;
			if(p != marker || !ignore_empty_fields){
				argv[argc]=marker;
				arg_idx[argc]=marker-args;
				argc++;
			}
			marker=p+1;
		}
		p++;
	}
	if(p != marker){
		argv[argc]=marker;
		arg_idx[argc]=marker-args;
		argc++;
	}

	return argc;
}
