/*
 * clientStream.c
 *
 *  Modified on:October 8, 2022
 *      Author: Angelo Caravella
 * Exercise 2.1
 * Download serverStream.c and clientStream.c from the material of lecture 7 in Google Classroom, compile and run them locally (by using the loopback address 127.0.0.1 for IP communication). Exploit Wireshark to capture data exchanged between client and server and identify the TCP segments that contain the transferred data.
 * Change the application in order to avoid the communication of the constant string and use again Wireshark to capture exchanged data.
 */

/*#include <sys/types.h>*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PROTOPORT 5193        /* Default server port number */
#define LOCALHOST "127.0.0.1" /* Default server address */

int main(int argc, char *argv[])
{
    struct sockaddr_in sad; /* Struct to host the transport address of the remote socket */
    int sd;                 /* Socket descriptor              	  */
    int port;               /* Port number                  		  */
    int n;                  /* Number of read bytes                */
    char buf[1000];         /* Buffer for data reading and writing */
    char *host;

    /* Create a socket for stream oriented communication */
    sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        fprintf(stderr, "socket creation failed\n");
        exit(1);
    }

    /* Clean the memory area that will store the transport address of the remote socket (server) */
    memset((char *)&sad, 0, sizeof(sad));

    /* Set the transport address of the remote socket (server)) */
    sad.sin_family = AF_INET;
    if (argc > 2)
    {
        port = atoi(argv[2]);
    }
    else
    {
        port = PROTOPORT;
    }
    if (port > 0)
        sad.sin_port = htons((u_short)port);
    else
    {
        fprintf(stderr, "bad port number %s\n", argv[2]);
        exit(1);
    }

    /* Check for the existence of an IP address on the command line */
    if (argc > 1)
    {
        host = argv[1]; /* if host argument is specified */
    }
    else
    {
        host = LOCALHOST;
    }

    sad.sin_addr.s_addr = inet_addr(host);

    /* Connect the local socket (sd) with the remote one identified by the transport address sad */
    if (connect(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0)
    {
        fprintf(stderr, "connect failed\n");
        exit(1);
    }
    /* The response message is processed */
    /* Read the data from the network and write them onto the stdout */
    n = read(sd, buf, sizeof(buf));
    while (n > 0)
    {
        printf("This server has been contacted %d time%s\n", *(int *)buf, *(int *)buf == 1 ? "." : "s.");
        n = read(sd, buf, sizeof(buf));
    }
    /* The response message has been processed */

    /* Close the socket. */
    close(sd);

    exit(0);
}
