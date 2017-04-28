#ifndef _CMD_PROCESS_TEST_
#define _CMD_PROCESS_TEST_
#include "socket_communit.h"
#include <strings.h> 

void cmd_processor_test(struct request_node req_node){
    int i;
    int len;
   
    len=strlen(req_node.data);
    for(i=0;i<len; i++)
    {
        req_node.data[i]=toupper(req_node.data[i]);
    }
}
#endif