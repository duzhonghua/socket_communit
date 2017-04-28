
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <strings.h>
#include <ctype.h>


char * host_name = "127.0.0.1";
int port = 8000;

int main(int argc , char * argv[])
{
        char buf[8192];
        int i =0;
        //char message[256];
        int socket_descriptor;
        struct sockaddr_in pin;
        char * str ="0:abc";
        if(argc < 2)
        {
                printf("we will send a default test string.\n");

        }
        else
        {
                str = argv[1];
                if(argc == 3)
                {
                        host_name = argv[2];
                }
        }

        bzero(&pin,sizeof(pin));
        pin.sin_family = AF_INET;
        inet_pton(AF_INET,host_name,&pin.sin_addr);
        pin.sin_port = htons(port);
 printf("starting test\n");
        for(i = 0; i< 100000; i++){
            if(i%100 == 0)
                 printf("tested: %d\n", i);
            if((socket_descriptor =  socket(AF_INET,SOCK_STREAM,0)) == -1)
            {
                    perror("error opening socket \n");
                    exit(1);
            }
            if(connect(socket_descriptor,(struct sockaddr * )&pin,sizeof(pin)) == -1)
            {
                    perror("error connecting to socket \n");
                    exit(1);
            }

           // printf("sending message %s to server ..\n",str);
            if( write(socket_descriptor,str,strlen(str)+1) == -1 )
            {
                    perror("error in send \n");
                    exit(1);
            }

            //printf("..sent message ...wait for message..\n");
            if( read(socket_descriptor,buf,8192) == -1 )
            {
                    perror("error in receiving response from server \n");
                    exit(1);
            }

       //     printf("\nResponse from server:\n\n%s\n",buf);
            close(socket_descriptor);
        }

        printf("starting DONE\n");
        return 1;


}

