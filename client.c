// This C program opens a TCP connection on the port number mentioned
// at port.h file with the server address mentioned as the second
// paramter to the program while excution. This program takes 2 
// input parameters: first, the name of file that the client wants
// to save the data packets received from server; second, the address
// of the server where the client want to request the file from.
//
// Sample command for execution:
// gcc client.c -o client
// ./client file1 10.10.2.2
//     --> file1 is the name of file to save the data
//     --> 10.10.2.2 is the IP address of the server
//
// Debajyoti Dash


#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <math.h>
#include "port.h"
static int packetnum = 0;
static long long totalpackettime = 0;
static int bytercvd = 0;
int main(int argc, char *argv[])
{
    if (argc !=3){
        printf("No file name or IP address mentioned to store data transferred from server.\n");
        return -1;
    }
    struct timeval te;
    gettimeofday (&te, NULL);
    long long milliseconds1 = te.tv_sec*1000LL + te.tv_usec/1000;
    printf("\n1. Start time first data packet receive (in ms): [%d]\n", milliseconds1);
    int sockfd = 0;
    int bytesReceived = 0;
    char recvBuff[256];
    memset(recvBuff, '0', sizeof(recvBuff));
    struct sockaddr_in serv_addr;
    int total_bytes_transfered = 0;
    /* Create a socket first */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }
    /* Initialize sockaddr_in data structure */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVICE_PORT); // port
    serv_addr.sin_addr.s_addr = inet_addr(argv[2]);
    //serv_addr.sin_addr.s_addr = inet_addr("10.10.0.2");
    /* Attempt a connection */
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }
    /* Create file where data will be stored */
    FILE *fp;
    fp = fopen(argv[1], "ab"); 
    if(NULL == fp)
    {
        printf("Error opening file");
        return 1;
    }
    /* Receive data in chunks of 256 bytes */
    long long lastms = milliseconds1;
    while((bytesReceived = read(sockfd, recvBuff, 256)) > 0)
    {
        bytercvd = bytercvd + bytesReceived;
        //printf("Bytes received %d\n",bytesReceived);    
        // recvBuff[n] = 0;
        fwrite(recvBuff, 1,bytesReceived,fp);
         //printf("%s \n", recvBuff);
        struct timeval t;
        gettimeofday (&t, NULL);
        long long milliseconds3 = t.tv_sec*1000LL + t.tv_usec/1000;
        //printf("Time for packet [%i] (in milliseconds): %i\n", packetnum, milliseconds3);
        totalpackettime = totalpackettime + (milliseconds3 - lastms);
        lastms = milliseconds3;
        packetnum++;
    }
    packetnum = packetnum - 1;
    
    struct timeval t;
    gettimeofday (&t, NULL);
    long long milliseconds2 = t.tv_sec*1000LL + t.tv_usec/1000;
    printf("2. End time last data packet receive (in ms):    [%d]\n\n", milliseconds2);
    float rate;
    float estimated_time;
    long long total_time;
    total_time = milliseconds2-milliseconds1;
    printf("3. Actual time for file transfer (in ms):                   [%d]\n", milliseconds2-milliseconds1);
    printf("4. Number of packets (size 256 bytes) received by client:   [%i]\n", packetnum);
    printf("5. Cumulative time to transfer [%i] packets (in ms):      [%d]\n\n", packetnum, totalpackettime);
    
    //printf("Time taken by each packet (in ms): %.2lf\n", (totalpackettime/packetnum));
    
    if(bytercvd < 0)
    {
        printf("\n Read Error \n");
    }
    printf("6. File size (in bytes):                    [%d]\n\n", bytercvd);
    
    
    rate = ((double)totalpackettime)/(packetnum);
    printf("7. Average Rate at which the packets are transfered {(5)/(4)} (in ms):  [%.2lf]\n", rate);
    estimated_time =  (bytercvd/ ((double)256))*rate;
    printf("8. Estimated time {((6)/256)*(7)} as per rate[%.2lf] (in ms):            [%.2lf]\n", rate, estimated_time);
    printf("9. Difference between Actual Time and Estimated Time {(3)-(8)} (in ms): [%.2lf]\n\n", total_time - estimated_time);
    
    return 0;
}
