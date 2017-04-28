#ifndef _CMD_PROCESS_TEST_
#define _CMD_PROCESS_TEST_
#include "socket_communit.h"
#include <strings.h> 

void cmd_processor_test(char* buf){
    int i;
    int len;
   
    len=strlen(buf);
    for(i=0;i<len; i++)
    {
        buf[i]=toupper(buf[i]);
    }
}
#endif