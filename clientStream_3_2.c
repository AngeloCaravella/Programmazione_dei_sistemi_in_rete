/*
 * clientStream_3_2.c
 *
 * Modified on: October 12, 2022
 *      Author: Angelo Caravella
 * Exercise 3.2
Write an application to implement a peer-to-peer chat between two processes.
An application for chatting is intrinsically peer-to-peer (each process can send and receive sequences of characters).
The stream-oriented communication channel built atop TCP is full-duplex: communication is bidirectional. On the other
hand, the socket API provides some functions to implement the client/server model: a process works in the role of
server to listen client connections; the other one starts the interaction. It is worth noting that these two roles characterize
the two channel endpoints (sockets) only for building the communication channel. When the channel is built, the two
communicating processes can assume different roles during the conversation: when a process (A) sends a message, the
other process (B) should be ready to read data and vice versa. Since read() is a blocking function, a half-duplex protocol
could be useful to perform bidirectional write/read interactions by suspending one process at a time. According to the
half-duplex protocol, while a process writes on the network channel the other ones reads. A state variable and a Finite
State Machine can be used to coordinate behavior changes.
 * 
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
#define YOU 0
#define PEER 1
#define EXIT 2

int main(int argc, char *argv[])
{
    struct sockaddr_in sad; /* Struct to host the transport address of the remote socket */
    int sd;                 /* Socket descriptor              	  */
    int port;               /* Port number                  		  */
    int n;                  /* Number of read bytes                */
    char buf[1000];         /* Buffer for data reading and writing */
    char *host;
    int len = 0;
    char status;

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
    status = YOU; /*start state of client is you*/

    while (status != EXIT)
    {

        memset(buf, 0, sizeof(buf));

        switch (status)
        {
        case YOU:
            do
            {
                printf("you>: ");
                scanf("\n%[^\n]s", buf);
                len = strlen(buf);
                if (buf[len - 1] == '.')
                    status = EXIT;
                else if (buf[len - 1] == '-')
                    status = PEER;

                len = strlen(buf);
                write(sd, &len, sizeof(int));
                write(sd, buf, len + 1); /*write string with delimiter*/

            } while (status == YOU);
            break;

        /*status peer refer that client is reading from channel*/
        case PEER:
            do
            {
                printf("peer>: ");
                read(sd, &len, sizeof(int));
                n = 0;
                while (n < len + 1)
                    n += read(sd, buf + n, sizeof(buf) - n);

                if (buf[len - 1] == '.')
                    status = EXIT;
                else if (buf[len - 1] == '-')
                    status = YOU;
                printf("%s\n", buf);

            } while (status == PEER);
            break;
        }
    }
    printf("end comunication");

    /* Close the socket. */
    close(sd);

    exit(0);
}
