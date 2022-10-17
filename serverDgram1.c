/*
* ServerDgram1.c
*
*  Modified on: October 5, 2022
*      Author: Angelo Caravella
Exercise 1.1
* Download serverDgram.c and clientDgram.c from the material of lecture 6 in Google Classroom, compile and run them
* locally (by using the loopback address 127.0.0.1 for IP communication). Exploit Wireshark to capture data exchanged
* between client and server and identify the UDP datagrams that contain the transferred data.
* Change the application in order to avoid the communication of the constant string and use again Wireshark to capture
* exchanged data.
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
/*#include <unistd.h>*/

#define PROTOPORT 5193 /* Default server port number */

int main(int argc, char *argv[])
{
    char buf[256];
    int serverSocket;
    socklen_t clientAddrLen;
    int visits = 0;
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;

    /* Create a socket for datagram oriented communication */
    serverSocket = socket(PF_INET, SOCK_DGRAM, 0);

    /* Clean the memory area that will store the transport address of the local socket (server) */
    memset(&serverAddr, 0, sizeof(serverAddr));

    /* Set the transport address of the local socket */
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PROTOPORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Assign the transport address to the local socket (server) */
    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    clientAddrLen = sizeof(clientAddr);

    /* The continuous loop characterizes the permanent execution of the server */
    while (1)
    {
        /* Clean the memory area that will store the data to send (buf) */
        memset(buf, 0, sizeof(buf));

        /* Receive the data from the client and put them in buf */
        recvfrom(serverSocket, buf, sizeof(buf), 0, (struct sockaddr *)&clientAddr, &clientAddrLen);

        /* The request message is processed */
        /* The status variable (visits) is incremented and the response message is built */
        visits++;
        memset(buf, 0, sizeof(buf));
        //This is the previous code, that was written 
        //sprintf( buf, "This server has been contacted %d times%s\n",visits, visits==1 ? .".": "s.");
        
        *((int *)buf) = visits;
        /* Send the data by copying the data in buf to the system */
        sendto(serverSocket, buf, sizeof(buf), 0, (struct sockaddr *)&clientAddr, clientAddrLen);
    }
    return 0;
}
