/*
    PC based Monitoring System source file
    version 1.0
    by J.Hunter,jhunter@263.net,May 2001

    Server profile data report 
*/
#include "precomp.h"

char * up_time()
{
    time_t ltime;
    struct tm *thetime;
    static unsigned char strTm[100];
    static bool first=true;

    if(first){
        first=false;
    }else{
        return (char*)strTm;
    }
    
    time (&ltime);
    thetime = localtime(&ltime);

    strftime((char *)strTm, 100, "%#c", (const struct tm *)thetime);
    return (char*)strTm;
}
