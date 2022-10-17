/*
 * clientStream_3_2.c
 *
 * Modified on: October 13, 2022
 *      Author: Angelo Caravella

Change the chat application implemented with exercise 3.2 in order to fully exploit the TCP channel for simultaneous
bidirectional communication. To this end, we need to overcome the limitations due to the blocking behavior of some
sockets functions (e.g. accept(), read(), etc.). Asynchronous I/O based on the select() function is a possible solution. By
exploiting this function, differently from exercise 3.2, the state at each iteration will no longer be determined by the
special character ‘-‘ used to pass the control but will be inferred by the monitored events.
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
#define MAX(x, y) (((x) > (y)) ? (x) : (y)) /*define max between x and y*/

int main(int argc, char *argv[])
{
    struct sockaddr_in sad; /* Struct to host the transport address of the remote socket */
    int sd;                 /* Socket descriptor              	  */
    int port;               /* Port number                  		  */
    int n;                  /* Number of read bytes                */
    char buf[1000];         /* Buffer for data reading and writing */
    char *host;
    int len = 0;
    char status; /*manager of process state*/
    int maxfds;
    fd_set readfds;

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
    printf("start comunication write something \n");

    while (status != EXIT)
    {
        /*implements status of event*/
        FD_ZERO(&readfds);
        FD_SET(sd, &readfds);
        FD_SET(STDIN_FILENO, &readfds);

        maxfds = MAX(STDIN_FILENO, sd);
        select(maxfds + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(STDIN_FILENO, &readfds))
            status = YOU;
        else
            status = PEER;

        memset(buf, 0, sizeof(buf));

        switch (status)
        {
        case YOU:

            printf("you>: ");
            scanf("\n%[^\n]s", buf);
            len = strlen(buf);
            if (buf[len - 1] == '.')
                status = EXIT;

            write(sd, &len, sizeof(int));
            write(sd, buf, len + 1); /*write string with delimiter*/

            break;

        /*status peer refer that client is reading from channel*/
        case PEER:

            printf("peer>: ");
            read(sd, &len, sizeof(int));
            n = 0;
            while (n < len + 1)
                n += read(sd, buf + n, sizeof(buf) - n);

            if (buf[len - 1] == '.')
                status = EXIT;
            printf("%s", buf);

            break;
        }
    }
    printf("end comunication");

    /* Close the socket. */
    close(sd);

    exit(0);
}
