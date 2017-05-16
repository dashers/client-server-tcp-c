# client-server-tcp-c
A client and server implementation of tcp architecture in C.
This program treats rb1 as client and rb2 as server. client.c file is deployed in rb1 server and server.c file is deployed in rb2 server.
1.  port.h --> This file contains the port number that is used by rb1 and rb2 to communicate with each other.
2.  client.c --> This program takes 2 parameter as argument. First parameter is the name of the file where you want to download the data from server. If the file name is not present then its created by the code. Second parameter is the IPv4 address of the server. The program prints the start time when 1st data packet is received and end time when last data packet is received. Also at the end of the program the difference of the time is calculated and the total time for transfer is shown in screen output. Each time a data packet is received. Once the program successfully terminates the file provided as parameter to this C program is created in the same folder where this program is run.
3.  server.c --> This program takes 1 parameter as argument. This parameter is the name of the file that you want to transfer to the client. This program first opens the file that will be transferred to client. If the file is NULL or not present in the location then "File open error" is printed on screen and program terminates. When program starts, it keeps on looking for any request that is made to the port mentioned in port.h file. Once the program detects a request from client, the program breaks the file into chunks of 256 bytes. Each chunk or data packet is transmitted to client separately. The program prints the start time when 1st data packet is read/sent to client and end time when last data packet is read/sent to client. Also at the end of the program the difference of the time is calculated and the total time needed by server to reach and send the file is shown in screen output. Once the complete file is read and last data packet sent by server "End of file" is printed on the screen.

SETUP PROCEDURES:
-----------------
1. rb1 server: copy port.h and client.c files to rb1 server.
2. rb2 server: copy port.h and server.c files to rb2 server.
3. rb2 server: first compile server.c file --> gcc server.c -o server
4. rb2 server: choose the file that you wish to transfer to rb1 (client) --> lets say its "file1"
5. rb2 server: execute --> ./server file1
6. rb1 server: now compile client.c file --> gcc client.c -o client
7. rb1 server: choose the file that you wish to store all the data packets received from server rb2. If file is not present in directory then its created. But if the file is already present then data is appended to the file. --> lets say its "myfile". Also note the IPv4 address of the server. Lets say its 10.10.2.2
8. rb1 server: execute --> ./client myfile 10.10.2.2

Now you can see the output in both rb1 and rb2 servers showing the time required by rb2 server to send the data packets and rb1 to received the data packets.

NOTE: If there are error while running the program like "connection failed". Then you have to change the port number in port.h file which you have placed in both rb1 and rb2 servers. And then retry the procedures. OR you might have to just wait for some time (approx. 10 seconds) and then retry.
