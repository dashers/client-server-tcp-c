// This C program opens a TCP socket connection on the port number 
// mentioned at port.h file and listens to any client request made.
// This program takes 1 input parameter: the name of file that the 
// server wants to send.
//
// Sample command for execution:
// gcc server.c -o server
// ./server myfile
//     --> myfile is the name of file that will be send to client
//
// Debajyoti Dash


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include "port.h"
static int packetnum = 0;
int main(int argc, char *argv[])
{
    if (argc !=2){
        printf("No file name mentioned to transfer to client.\n");
        return -1;
    }
    int listenfd = 0;
    int connfd = 0;
    struct sockaddr_in serv_addr;
    char sendBuff[1025];
    int numrv;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket retrieve success\n");
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERVICE_PORT);
    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
    if(listen(listenfd, 10) == -1)
    {
        printf("Failed to listen\n");
        return -1;
    }
    int printinitialtime = 0;
    long long milliseconds1, milliseconds2;
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);
        /* Open the file that we wish to transfer */
        FILE *fp = fopen(argv[1],"rb");
        if(fp==NULL)
        {
            printf("File open error\n");
            return 1;   
        }   
        /* Read data from file and send it */
        while(1)
        {
            if(printinitialtime == 0){
                struct timeval te;
                gettimeofday (&te, NULL);
                milliseconds1 = te.tv_sec*1000LL + te.tv_usec/1000;
                printf("\n1. Start time reading first data packet (in ms): [%d]\n", milliseconds1);
                printinitialtime++;
            }
            /* First read file in chunks of 256 bytes */
            unsigned char buff[256]={0};
            int nread = fread(buff,1,256,fp);
            packetnum++;
            //printf("Bytes read %d", nread);        
            /* If read was success, send data. */
            if(nread > 0)
            {
                //printf("  ...Sending \n");
                write(connfd, buff, nread);
                
            }
            /*
             * There is something tricky going on with read .. 
             * Either there was error, or we reached end of file.
             */
            if (nread < 256)
            {
                if (feof(fp)){
                    struct timeval t;
                    gettimeofday (&t, NULL);
                    milliseconds2 = t.tv_sec*1000LL + t.tv_usec/1000;
                    printf("2. End time reading last data packet (in ms):    [%d]\n", milliseconds2);
                    printf("3. End of file\n\n");
                }
                if (ferror(fp))
                    printf("Error reading\n");
                break;
            }
            }    
        
        printf("4. Total Time taken by server (in ms):           [%d]\n\n", milliseconds2-milliseconds1);
        close(connfd);
        sleep(1);
        
    }
    return 0;
}
