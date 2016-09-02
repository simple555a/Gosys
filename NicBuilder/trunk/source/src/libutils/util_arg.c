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

int UTILS_API parse_arg_exA(const char * cmd_line,CArgs * args,char * delimiter)
{
    int i;
	char in_quotation;
	char * p,*marker;

#undef utils_argv
#undef utils_argc

#define utils_argv (args->argv)
#define utils_argc (args->argc)

    if(!delimiter) delimiter=" \n";

    strncpy(args->args,cmd_line,sizeof(args->args));
	args->args[sizeof(args->args)-1]=0;
    for(i=0;i<MAX_ARGC;i++) utils_argv[i]="";
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
			if(p != marker){
				args->argv[utils_argc]=marker;
				args->arg_idx[utils_argc]=marker-args->args;
				utils_argc++;
			}
			marker=p+1;
		}
        p++;
    }
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
    parse_arg_ex(",,,,,",&args,",");
    得到零个参数，因为strtok将所有的‘,’都忽略了
    而parse_arg_ey(",,,,,",&args,",");
    则得到5个参数，每个参数都是空字符串
*/
int UTILS_API parse_arg_eyA(const char * cmd_line,CArgs * args,char * delimiter)
{
    int i;
	char in_quotation;
	char * p,*marker;

#undef utils_argv
#undef utils_argc

#define utils_argv (args->argv)
#define utils_argc (args->argc)
    if(!delimiter) delimiter=" \n";

    strncpy(args->args,cmd_line,sizeof(args->args));
	args->args[sizeof(args->args)-1]=0;
    for(i=0;i<MAX_ARGC;i++) {
		utils_argv[i]="";
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
		}else if(strchr(delimiter,*p)){
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

LIBUTILS_API int UTILS_API process_argsA(int c, char **v, CCmdLineArgA * CCmdLineArgValues)
{
	return process_args_exA(c, v, CCmdLineArgValues, 1, 0);
}
/*
    Parse the command line according to specified pattern
    returns :
     0 -- OK
     -1 -- invalid parameter
     -2 -- /? -? /help or -help specified
*/
LIBUTILS_API  int UTILS_API process_args_exA(
	int c, 
	char **v, 
	CCmdLineArgA * CCmdLineArgValues, 
	int startArgOffset, 
	int reserved
	)
{
    CCmdLineArgA * arg;
    char * flag;
	int i;
	char * parg;

    for(i=startArgOffset; i<c; i++){
        flag=v[i];
        if(v[i][0] == '-' || v[i][0] == '/' ){
			if( *flag == '?' || !strncmp("help",flag,4) ){
				return UTILS_HELP_NEEDED;
			}
		}
        //search in CCmdLineArgValues global variable
        arg=CCmdLineArgValues;
        while(arg->flag){
            if( !strncmp(flag,arg->flag,strlen(arg->flag)) )
                break;
            arg++;
        }
        if( !arg->flag ){
            // utils_errorA("Unknown option -%s\n",flag);
            //print_usage();
            return UTILS_INVALID_PATTERN;
        }else{
			// 2003/11/11, Jackie, supports quoted argument
			parg = v[i]+strlen(arg->flag);
			if(*parg == '"' && strlen(parg) >= 2){
				if(parg[strlen(parg) - 1] == '"'){
					parg[strlen(parg) - 1] = 0;
					parg++;
				}
			}
            if( !strcmp(arg->mask,"unsigned int") ){
                *((unsigned int *)arg->value)=(unsigned)atoi(parg);
            }else if( !strcmp(arg->mask,"int")){
                *((int *)arg->value)=atoi(parg);
            }else if( !strcmp(arg->mask,"float")){
                *((float *)arg->value)=(float)atof(parg);
            }else if( !strcmp(arg->mask,"double")){
                *((double *)arg->value)=(double)atof(parg);
            }else if( !strcmp(arg->mask ,"char *")){
                *((char **)arg->value)=parg;
            }else if( !strcmp(arg->mask , "unsigned char *")){
                *((char **)arg->value)=parg;
            }else if( !strcmp(arg->mask,"bool")){
                char f=*(parg);
                switch(f){
                case '+':
                case '1':
                    *((char*)arg->value)=TRUE;
                    break;
                case '-':
                case '0':
                    *((char*)arg->value)=FALSE;
                }
            }else if( !strcmp(arg->mask,"tbool")){
                char f=*(parg);
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

void UTILS_API print_argumentsA(const CCmdLineArgA * CCmdLineArgValues)
{
    CCmdLineArgA * arg;

    arg=(CCmdLineArgA *)CCmdLineArgValues;
    utils_errorA("Arguments:\n");
    while( arg->flag ){
        if( !strcmp(arg->mask,"unsigned int") ){
            utils_traceA("%s(uint)=%d\n",arg->flag,*((unsigned int*)arg->value));
        }else if( !strcmp(arg->mask,"int")){
            utils_traceA("%s(int)=%d\n",arg->flag,*((int*)arg->value));
        }else if( !strcmp(arg->mask,"float")){
            utils_traceA("%s(float)=%f\n",*((float *)arg->value));
        }else if( !strcmp(arg->mask,"double")){
            utils_traceA("%s(double)=%f\n",arg->flag,*((double *)arg->value));
        }else if( !strcmp(arg->mask ,"char *")){
            utils_traceA("%s(char*)='%s'",arg->flag,arg->value);
        }else if( !strcmp(arg->mask , "unsigned char *")){
            utils_traceA("%s(char*)='%s'",arg->flag,arg->value);
        }else if( !strcmp(arg->mask,"char")){
            utils_traceA("%s(char)=%s",*((char*)arg->value)? "TRUE" : "FALSE");
        }else if( !strcmp(arg->mask,"tbool")){
            utils_traceA("%s(char)=%s",*((char*)arg->value)? "TRUE" : "FALSE");
        }else if( !strcmp(arg->mask,"bool")){
            utils_traceA("%s(bool)=%s",*((char*)arg->value)? "TRUE" : "FALSE");
        }else{
		utils_errorA("Unknown mask : %s\n",arg->mask);
        }
        arg++;
    }    
}

void UTILS_API print_usageA(const CCmdLineArgA * CCmdLineArgValues)
{
    CCmdLineArgA * arg;

    arg=(CCmdLineArgA *)CCmdLineArgValues;
    utils_errorA("Options:\n");
    while( arg->flag ){
        if( !strcmp(arg->mask,"unsigned int") ){
            utils_traceA("\t%s<%s>\t%s\n",arg->flag,"uint",arg->hint);
        }else if( !strcmp(arg->mask,"int")){
            utils_traceA("\t%s<%s>\t%s\n",arg->flag,"int",arg->hint);
        }else if( !strcmp(arg->mask,"float")){
            utils_traceA("\t%s<%s>\t%s\n",arg->flag,"float",arg->hint);
        }else if( !strcmp(arg->mask,"double")){
            utils_traceA("\t%s<%s>\t%s\n",arg->flag,"double",arg->hint);
        }else if( !strcmp(arg->mask ,"char *")){
            utils_traceA("\t%s<%s>\t%s\n",arg->flag,"string",arg->hint);
        }else if( !strcmp(arg->mask , "unsigned char *")){
            utils_traceA("\t%s<%s>\t%s\n",arg->flag,"string",arg->hint);
        }else if( !strcmp(arg->mask,"char")){
            utils_traceA("\t%s\t%s\n",arg->flag,arg->hint);
        }else if( !strcmp(arg->mask,"tbool")){
            utils_traceA("\t%s\t%s\n",arg->flag,arg->hint);
        }else if( !strcmp(arg->mask,"bool")){
            utils_traceA("\t%s\t%s\n",arg->flag, arg->hint);
        }else{
			utils_errorA("Unknown mask : %s\n",arg->mask);
        }
        arg++;        
    }
    utils_errorA("\t-? | -help\tshow this screen.\n");
    // exit(0);
}
