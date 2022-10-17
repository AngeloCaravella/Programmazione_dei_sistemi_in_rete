/*
 * serverStream.c
 *
 *  Modified on: October 8, 2022
 *      Author: Angelo Caravella
 * Exercise 2.1
 * Download serverStream.c and clientStream.c from the material of lecture 7 in Google Classroom, compile and run them locally (by using the loopback address 127.0.0.1 for IP communication). Exploit Wireshark to capture data exchanged between client and server and identify the TCP segments that contain the transferred data.
 * Change the application in order to avoid the communication of the constant string and use again Wireshark to capture exchanged data.
 */
/*#include <sys/types.h>*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PROTOPORT 5193 /* Default port number */
#define QLEN 6         /* Size of connection requests queue   */

int main(int argc, char *argv[])
{
  struct sockaddr_in sad; /* Struct to store the transport address of the server socket */
  struct sockaddr_in cad; /* Struct to store the transport address of the client socket */
  int sd, sd2;            /* Socket descriptors */
  int port;               /* Port number */
  socklen_t alen;         /* Lenght of the client transport address  */
  char buf[1000];         /* Buffer for sending and receiving data */
  int visits = 0;         /* Status variable to count the number of connections from clients */

  /* Create a socket for stream oriented communication */
  sd = socket(PF_INET, SOCK_STREAM, 0);
  if (sd < 0)
  {
    fprintf(stderr, "socket creation failed\n");
    exit(1);
  }
  printf("Socket id: %d\n", sd);
  /* Clean the memory area that will store the transport address of the local socket (server) */
  memset((char *)&sad, 0, sizeof(sad));

  /* Set the transport address of the local socket (server) */
  sad.sin_family = AF_INET;
  sad.sin_addr.s_addr = htonl(INADDR_ANY);
  if (argc > 1)
  {
    port = atoi(argv[1]);
  }
  else
  {
    port = PROTOPORT;
  }
  sad.sin_port = htons((u_short)port);

  /* Bind a transport address (sad) to the local socket (sd) */
  if (bind(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0)
  {
    fprintf(stderr, "bind failed\n");
    exit(1);
  }

  /* Characterize the socket as a welcome socket (server) */
  if (listen(sd, QLEN) < 0)
  {
    fprintf(stderr, "listen failed\n");
    exit(1);
  }

  alen = sizeof(cad);

  while (1)
  {

    /* Extract a connection request from the queue and create a new socket */
    if ((sd2 = accept(sd, (struct sockaddr *)&cad, &alen)) < 0)
    {
      fprintf(stderr, "accept failed\n");
      exit(1);
    }
    visits++;
    *((int *)buf) = visits;
    /* Write the response message in the network channel */
    write(sd2, buf, strlen(buf));

    /* Close the connection socket created by accept() */
    close(sd2);
  }
}
