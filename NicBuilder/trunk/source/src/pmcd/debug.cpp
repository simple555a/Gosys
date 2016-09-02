/*
    PC based Monitoring System source file
    version 1.0
    by J.Hunter,jhunter@263.net,May 2001    
*/
#include "precomp.h"

static int foo_utils_out(char *,...)
{
    return 0; 
}

// save old procedure
static int (*utils_out_procs[3])(char *,...);

int on_debug(char * cmd,void *)
{    
    if(!utils_out_procs[0]){
        utils_out_procs[0]=utils_trace;
        utils_out_procs[1]=utils_debug;
        utils_out_procs[2]=utils_error;
    };
    int res;
    if(!strcmp(cmd,"off")){
        utils_debug=foo_utils_out;
        res=1;
    }else if(!strcmp(cmd,"on")){
        utils_debug=utils_out_procs[1];
        res=1;
    }else{
        res=0;
    }
    return res;
};

int on_trace(char * cmd,void *)
{
    if(!utils_out_procs[0]){
        utils_out_procs[0]=utils_trace;
        utils_out_procs[1]=utils_debug;
        utils_out_procs[2]=utils_error;
    };
    int res;
    if(!strcmp(cmd,"off")){
        utils_trace=foo_utils_out;
        res=1;
    }else if(!strcmp(cmd,"on")){
        utils_trace=utils_out_procs[0];
        res=1;
    }else{
        res=0;
    }
    return res;
};

int on_error(char * cmd,void *)
{
    if(!utils_out_procs[0]){
        utils_out_procs[0]=utils_trace;
        utils_out_procs[1]=utils_debug;
        utils_out_procs[2]=utils_error;
    };
    int res;
    if(!strcmp(cmd,"off")){
        utils_error=foo_utils_out;
        res=1;
    }else if(!strcmp(cmd,"on")){
        utils_debug=utils_out_procs[2];
        res=1;
    }else{
        res=0;
    }
    return res;
};
