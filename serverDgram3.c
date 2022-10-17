/*
 * ServerDgram3.c
 *
 *  Modified on: October , 2022
 *      Author: Angelo Caravella
 * Exercise 1.3
 * Change the code of the first exercise such that the client sends the name of the user (read from the stdin) in the request message and change the response message in order to contain also the name of the user.
 * The client should show a message like the following: “Eugenio, you are the 6° user of this server”, where Eugenio is the name passed by the client and 6 is the number of contacts received by the server.
 * Exploit Wireshark to analyze the content of the message exchanged.

 */

/*#include <sys/types.h>*/
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
         
        sprintf(buf, "%s ,you are the %d user of this server", buf , visits );
        /* The response message has been produced */

        /* Send the data by copying the data in buf to the system */
        sendto(serverSocket, buf, sizeof(buf), 0, (struct sockaddr *)&clientAddr, clientAddrLen);
    }
    return 0;
}