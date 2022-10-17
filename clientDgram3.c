/*
 * clientDgram3.c
 *
 *  Modified on: October 4, 2022
 *      Author: Angelo Caravella
 * Exercise 1.3
 * Change the code of the first exercise such that the client sends the name of the user (read from the stdin) in the request message and change the response message in order to contain also the name of the user.
 * The client should show a message like the following: “Eugenio, you are the 6° user of this server”, where Eugenio is the name passed by the client and 6 is the number of contacts received by the server.
 * Exploit Wireshark to analyze the content of the message exchanged.

 */

/*#include <sys/types.h>*/
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

    serverAddrLen = sizeof(serverAddr);

    /* Clean the memory area that will store the data to send */
    memset(buf, 0, sizeof(buf));

    printf("Please insert the name of the user:\n");
    scanf("%s", buf);

    /* Send the data */
    sendto(clientSocket, buf, sizeof(buf), 0, (struct sockaddr *)&serverAddr, serverAddrLen);

    memset(buf, 0, sizeof(buf));

    /* Receive the data from the server; we assume that the data are encapsulated in a single message */
    recvfrom(clientSocket, buf, sizeof(buf), 0, (struct sockaddr *)&serverAddr, &serverAddrLen);

    /* Show the data on the stdout */
    printf("%s\n", buf);

    /* Close the socket and release the related resources */
    close(clientSocket);
    return 0;
}