#include <sys/socket.h>  
#include <sys/types.h>  
#include <unistd.h>   
#include <arpa/inet.h>  
#include <netinet/in.h>  
#include <errno.h>  
#include <strings.h>  
#include <ctype.h> 
#include "cmd_processor.h"
#define RESULT_MAX_SIZE 256

char host_name[20];
int port = 8000;

static int PROCESSOR_CNT = sizeof(PROCESSORS)/sizeof(struct porcessor_node);


CMD parase_cmd(char* buf){
    char int_cmd[8];
    char* cmd_index = strchr(buf, ':');
    log_out("parase_cmd buf %s\n",buf);
    strncpy(int_cmd, buf, cmd_index-buf);
    log_out("parase_cmd cmd:%s\n",int_cmd);
    int icmd = atol(int_cmd);
    log_out("parase_cmd int cmd:%d\n",icmd);
    return icmd;
}

char * parase_data(char * buf){
    return strchr(buf, ':')+1;
}


void process_result(struct request_node node){
    int len = strlen(node.data);
    if(send(node.socket,node.data,len+1,0) == -1)
    {
        perror("call to send");
        exit(1);
    }
    close(node.socket);
}

void process_cmd(struct request_node node){
    struct porcessor_node *pn;
    char * result = NULL;
    int i = 0;
    for(i = 0; i < PROCESSOR_CNT; i++){
        pn = &PROCESSORS[i];
         log_out("process_cmd found cmd processor\n");
         if(pn->cmd == node.cmd){
            if(pn->porcessor!=NULL){
                pn->porcessor(node.data);
                process_result(node);
            }
            break;
        }     
    }
}


int parase_process_cmd(int socket, char *buf){
    log_out("parase_process_cmd\n");
    if(buf ==0)
        return -1;

    // create new cmd_node
    struct request_node node;
    node.cmd = parase_cmd(buf);
    node.data = parase_data(buf);
    log_out("parase_process_cmd data\n");
    log_out("parase_process_cmd data:%s\n", node.data);
    node.socket = socket;
    process_cmd(node);
    return 0;
}



int main()
{   
    struct sockaddr_in sin,pin;
    int sock_descriptor,temp_sock_descriptor,address_size;
    int i , len , on=1;
    char buf[16384];

    sock_descriptor = socket(AF_INET,SOCK_STREAM,0);
    bzero(&sin,sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);
    if(bind(sock_descriptor,(struct sockaddr *)&sin,sizeof(sin)) == -1)
    {
            perror("call to bind");
            exit(1);
    }
    if(listen(sock_descriptor,100) == -1)
    {
            perror("call to listem");
            exit(1);
    }
    log_out("Accpting connections...\n");

    while(1)
    {
        // receive socket connections
        address_size = sizeof(pin);
        temp_sock_descriptor = accept(sock_descriptor,(struct sockaddr *)&pin,&address_size);
        if(temp_sock_descriptor == -1)
        {
            perror("call to accept");
            exit(1);
        }
        // receive from peer
        if(recv(temp_sock_descriptor,buf,16384,0) == -1)
        {
            perror("call to recv");
            exit(1);
        }

        // save log
        inet_ntop(AF_INET,&pin.sin_addr,host_name,sizeof(host_name));
        log_out("received from client(%s): %s\n",host_name,buf);

        // add socket_request to cmd parase
        parase_process_cmd(temp_sock_descriptor, buf);
    }
}
