/*
    PC based Monitoring System source file
    version 1.0
    by J.Hunter,jhunter@263.net,May 2001 

    J.Hunter, 2002/5/27, parse_arg_ex and parse_arg_ey now support 
    \" quoted sequence
*/
#pragma warning(disable:4996)
#include "precomp.h"
#pragma hdrstop 

LIBUTILS_API int UTILS_API parse_arg_exW(const wchar_t * cmd_line, CArgsW * args,wchar_t * delimiter)
{
    int i;
	char in_quotation;
	wchar_t * p,*marker;

#undef utils_argv
#undef utils_argc

#define utils_argv (args->argv)
#define utils_argc (args->argc)

    if(!delimiter) delimiter=L" \n";

    wcsncpy(args->args,cmd_line,sizeof(args->args));
	args->args[sizeof(args->args)-1]=0;
	for(i=0;i<MAX_ARGC;i++) {
		utils_argv[i]=L"";
	}
    utils_argc=0;
    
    utils_argc=0; 
    p=marker=args->args;
	in_quotation=FALSE;
    while(*p){
		if(in_quotation){
			// in quatation mode, all demiliters are ignored
			// until a matching '"' is found
			if('"' == *p){
				in_quotation = FALSE;
				if(*marker == '"' && (wcschr(delimiter,p[1]) || !p[1])){
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
		}else if(wcschr(delimiter,*p)){
			// got an argument, emit it to the argument buffer
			*p=0;
			if(p != marker){
				args->argv[utils_argc]=marker;
				args->arg_idx[utils_argc]=marker-args->args;
				utils_argc++;
			}
			marker=p+1;
		}
        p++;
    };
	if(p != marker){
		args->argv[utils_argc]=marker;
		args->arg_idx[utils_argc]=marker-args->args;
		utils_argc++;
	}
    return utils_argc;
}

/*
    parse_arg_ey 和 parse_arg_ex 不同，在于它将连续出现的分割符号的处理方式不同
    举例来说：
    parse_arg_ex(L",,,,,L",&args,L",L");
    得到零个参数，因为strtok将所有的‘,’都忽略了
    而parse_arg_ey(L",,,,,L",&args,L",L");
    则得到5个参数，每个参数都是空字符串
*/
LIBUTILS_API int UTILS_API parse_arg_eyW(const wchar_t * cmd_line,CArgsW * args,wchar_t * delimiter)
{
    int i;
	char in_quotation;
	wchar_t * p,*marker;

#undef utils_argv
#undef utils_argc

#define utils_argv (args->argv)
#define utils_argc (args->argc)
    if(!delimiter) delimiter=L" \n";

    wcsncpy(args->args,cmd_line,sizeof(args->args));
	args->args[sizeof(args->args)-1]=0;
    for(i=0;i<MAX_ARGC;i++) {
		utils_argv[i]=L"";
	}
    
    utils_argc=0; 
    p=marker=args->args;
	in_quotation = FALSE;
    while(*p){
        if(in_quotation){
			// in quatation mode, all demiliters are ignored
			// until a matching '"' is found
			if('"' == *p){
				in_quotation = FALSE;
			}
		}else if('"' == *p){
			in_quotation = TRUE;
		}else if(wcschr(delimiter,*p)){
            *p=0;
            args->argv[utils_argc]=marker;
            args->arg_idx[utils_argc]=marker-args->args;
            utils_argc++;
            marker=p+1;
        };
        p++;
    };
    args->argv[utils_argc]=marker;
    args->arg_idx[utils_argc]=marker-args->args;
    utils_argc++;
    return utils_argc;
}

LIBUTILS_API int UTILS_API process_argsW(int c, wchar_t **v, CCmdLineArgW * CCmdLineArgValues)
{
	return process_args_exW(c, v, CCmdLineArgValues, 1, 0);
}

static double _my_wtof(wchar_t * a)
{
	char buf[32];
	WideCharToMultiByte(
		CP_ACP,
		0,
		a,
		-1,
		buf,
		sizeof(buf),
		NULL,
		NULL
		);
	return atof(buf);
}

/*
    Parse the command line according to specified pattern
    returns :
     0 -- OK
     -1 -- invalid parameter
     -2 -- /? -? /help or -help specified
*/
LIBUTILS_API  int UTILS_API process_args_exW(
	int c, 
	wchar_t **v, 
	CCmdLineArgW * CCmdLineArgValues, 
	int startArgOffset, 
	int reserved
	)
{
    CCmdLineArgW * arg;
    wchar_t * flag;
	int i;
	wchar_t * parg;
	wchar_t f;

    for(i=startArgOffset; i<c; i++){
        flag=v[i];
        if(v[i][0] == '-' || v[i][0] == '/' ){
			if( *flag == '?' || !wcsncmp(L"help",flag,4) ){
				return UTILS_HELP_NEEDED;
			}
		}
        //search in CCmdLineArgValues global variable
        arg=CCmdLineArgValues;
        while(arg->flag){
            if( !wcsncmp(flag, arg->flag, wcslen(arg->flag)) )
                break;
            arg++;
        }
        if( !arg->flag ){
            // utils_errorW(L"Unknown option -%s\n",flag);
            //print_usage();
            return UTILS_INVALID_PATTERN;
        }else{
			// 2003/11/11, Jackie, supports quoted argument
			parg = v[i]+wcslen(arg->flag);
			if(*parg == '"' && wcslen(parg) >= 2){
				if(parg[wcslen(parg) - 1] == '"'){
					parg[wcslen(parg) - 1] = 0;
					parg++;
				}
			}
            if( !wcscmp(arg->mask,L"unsigned int") ){
                *((unsigned int *)arg->value)=(unsigned)_wtoi(parg);
            }else if( !wcscmp(arg->mask,L"int")){
                *((int *)arg->value)=_wtoi(parg);
            }else if( !wcscmp(arg->mask,L"float")){
                *((float *)arg->value)=(float)_my_wtof(parg);
            }else if( !wcscmp(arg->mask,L"double")){
                *((double *)arg->value)=(double)_my_wtof(parg);
            }else if( !wcscmp(arg->mask ,L"char *")){
                *((wchar_t **)arg->value)=parg;
            }else if( !wcscmp(arg->mask,L"unsigned char *")){
                *((wchar_t **)arg->value)=parg;
            }else if( !wcscmp(arg->mask,L"bool")){
                f=*(parg);
                switch(f){
                case '+':
                case '1':
                    *((char*)arg->value)=TRUE;
                    break;
                case '-':
                case '0':
                    *((char*)arg->value)=FALSE;
                }
            }else if( !wcscmp(arg->mask,L"tbool")){
               f=*(parg);
                switch(f){
                case '-':
                case '0':
                    *((char*)arg->value)=FALSE;
					break;
				default:
					*((char*)arg->value)=TRUE;
                }
            }
        }//if(pFlag)
    }
    return UTILS_SUCCESS;
}

LIBUTILS_API void UTILS_API print_argumentsW(const CCmdLineArgW * CCmdLineArgValues)
{
    CCmdLineArgW * arg;

    arg=(CCmdLineArgW *)CCmdLineArgValues;
    utils_errorW(L"Arguments:\n");
    while( arg->flag ){
        if( !wcscmp(arg->mask,L"unsigned int") ){
            utils_traceW(L"%s(uint)=%d\n",arg->flag,*((unsigned int*)arg->value));
        }else if( !wcscmp(arg->mask,L"int")){
            utils_traceW(L"%s(int)=%d\n",arg->flag,*((int*)arg->value));
        }else if( !wcscmp(arg->mask,L"float")){
            utils_traceW(L"%s(float)=%f\n",*((float *)arg->value));
        }else if( !wcscmp(arg->mask,L"double")){
            utils_traceW(L"%s(double)=%f\n",arg->flag,*((double *)arg->value));
        }else if( !wcscmp(arg->mask ,L"wchar_t *")){
            utils_traceW(L"%s(char*)='%s'",arg->flag,arg->value);
        }else if( !wcscmp(arg->mask , L"unsigned char *")){
            utils_traceW(L"%s(char*)='%s'",arg->flag,arg->value);
        }else if( !wcscmp(arg->mask,L"char")){
            utils_traceW(L"%s(char)=%s",*((char*)arg->value)? "TRUE" : "FALSE");
        }else if( !wcscmp(arg->mask,L"tbool")){
            utils_traceW(L"%s(char)=%s",*((char*)arg->value)? "TRUE" : "FALSE");
        }else{
			utils_errorW(L"Unknown mask : %s\n",arg->mask);
        }
        arg++;
    }    
}

LIBUTILS_API void UTILS_API print_usageW(const CCmdLineArgW * CCmdLineArgValues)
{
    CCmdLineArgW * arg;

    arg=(CCmdLineArgW *)CCmdLineArgValues;
    utils_errorW(L"Options:\n");
    while( arg->flag ){
        if( !wcscmp(arg->mask,L"unsigned int") ){
            utils_traceW(L"\t%s<%s>\t%s\n",arg->flag,L"uint",arg->hint);
        }else if( !wcscmp(arg->mask,L"int")){
            utils_traceW(L"\t%s<%s>\t%s\n",arg->flag,L"int",arg->hint);
        }else if( !wcscmp(arg->mask,L"float")){
            utils_traceW(L"\t%s<%s>\t%s\n",arg->flag,L"float",arg->hint);
        }else if( !wcscmp(arg->mask,L"double")){
            utils_traceW(L"\t%s<%s>\t%s\n",arg->flag,L"double",arg->hint);
        }else if( !wcscmp(arg->mask ,L"wchar_t *")){
            utils_traceW(L"\t%s<%s>\t%s\n",arg->flag,L"string",arg->hint);
        }else if( !wcscmp(arg->mask , L"unsigned wchar_t *")){
            utils_traceW(L"\t%s<%s>\t%s\n",arg->flag,L"string",arg->hint);
        }else if( !wcscmp(arg->mask,L"char")){
            utils_traceW(L"\t%s\t%s\n",arg->flag,arg->hint);
        }else if( !wcscmp(arg->mask,L"tbool")){
            utils_traceW(L"\t%s\t%s\n",arg->flag,arg->hint);
        }else{
			utils_errorW(L"Unknown mask : %s\n",arg->mask);
        }
        arg++;        
    }
    utils_errorW(L"\t-? | -help\tshow this screen.\n");
    // exit(0);
}
