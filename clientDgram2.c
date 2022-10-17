/*
* clientDgram2.c
*
* Modified on: October 5, 2022
* Author: Angelo Caravella
* Exercise 1.2
* Write a client/server application to remotely multiply two integer values. The client must read from the standard input
* two integer numbers and send them to the server. The server, after the reception of the two numbers, multiplies them and
* sends the result to the client. Use the datagram-oriented communication model (UDP). Exploit Wireshark to capture data
* exchanged between client and server and identify the UDP datagrams that contain the transferred data.
***
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PROTOPORT 5193        /* Default server port number */
#define LOCALHOST "127.0.0.1" /* Default server address */

int main(int argc, char *argv[])
{
        char buf[256];
        int clientSocket;
        struct sockaddr_in serverAddr;
        socklen_t serverAddrLen;

        char *host;

        /* Check for the existence of an IP address on the command line */
        if (argc > 1)
        {
                host = argv[1]; /* if the parameter has been passed */
        }
        else
        {
                host = LOCALHOST;
        }

        /* Create a socket for datagram oriented communication */
        clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

        /* Clean the memory area that will store the transport address of the remote socket (server) */
        memset(&serverAddr, 0, sizeof(serverAddr));

        /* Set the transport address of the remote socket (server) */
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(PROTOPORT);
        serverAddr.sin_addr.s_addr = inet_addr(host);
        int a;
        int b;

        serverAddrLen = sizeof(serverAddr);

        /* Clean the memory area that will store the data to send */
        memset(buf, 0, sizeof(buf));

        printf("Please give me two integers\n");

        printf("Please give me the first one \n");
        scanf("%d", (int *)buf);

        printf("Please give me the second one \n");
        scanf("%d", (int *)(buf + sizeof(int)));

        sendto(clientSocket, buf, 2 * sizeof(buf), 0, (struct sockaddr *)&serverAddr, serverAddrLen);

        memset(buf, 0, sizeof(buf));
        recvfrom(clientSocket, buf, sizeof(long), 0, (struct sockaddr *)&serverAddr, &serverAddrLen);

        printf("The product is: %ld\n", *(long *)buf);
        /* Close the socket and release the related resources */

        close(clientSocket);
        return 0;
}
