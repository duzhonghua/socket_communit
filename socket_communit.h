#ifndef _SOCKET_COMMUNIT__
#define _SOCKET_COMMUNIT__
//#define DEBUG
typedef enum{
	CMD_1 = 0,
	CMD_2,
	CMD_3,
	CMD_4,
	CMD_MAX
	}CMD;



struct request_node{
    int socket;
    CMD cmd;
    char* data;
}request_node;

typedef void (*cmd_processor)(char* buf);
struct porcessor_node{
    CMD cmd;
    cmd_processor porcessor;
};



#ifdef DEBUG
#define log_out(fmt, args...) printf(fmt, ##args)
#else
#define  log_out(fmt, ...)
#endif

#endif
