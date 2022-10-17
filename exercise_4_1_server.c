/*
 * serverStream_3_2.c
 *
 *
 *      Author: Angelo Caravella
 *  Modified on: October 13, 2022

Change the chat application implemented with exercise 3.2 in order to fully exploit the TCP channel for simultaneous
bidirectional communication. To this end, we need to overcome the limitations due to the blocking behavior of some
sockets functions (e.g. accept(), read(), etc.). Asynchronous I/O based on the select() function is a possible solution. By
exploiting this function, differently from exercise 3.2, the state at each iteration will no longer be determined by the
special character ‘-‘ used to pass the control but will be inferred by the monitored events.
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

#define YOU 0
#define PEER 1
#define EXIT 2

int main(int argc, char *argv[])
{
  struct sockaddr_in sad; /* Struct to store the transport address of the server socket */
  struct sockaddr_in cad; /* Struct to store the transport address of the client socket */
  int sd, sd2;            /* Socket descriptors */
  int port;               /* Port number */
  socklen_t alen;         /* Lenght of the client transport address  */
  char buf[1000];         /* Buffer for sending and receiving data */
  int visits = 0;         /* Status variable to count the number of connections from clients */
  int len = 0;
  int n = 0;
  char status;
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

    status = PEER; /*start state of client is you*/

    while (status != EXIT)
    {

      memset(buf, 0, sizeof(buf));

      switch (status)
      {
      case YOU:
        do
        {
          printf(" you>: ");
          scanf("\n%[^\n]s", buf);
          len = strlen(buf);
          if (buf[len - 1] == '.')
            status = EXIT;
          else if (buf[len - 1] == '-')
            status = PEER;

          write(sd2, &len, sizeof(int));
          write(sd2, buf, len + 1); /*write string with delimiter*/

        } while (status == YOU);
        break;

      /*status peer refer that client is reading from channel*/
      case PEER:
        do
        {
          printf("peer>: ");
          read(sd2, &len, sizeof(int));
          n = 0;
          while (n < len + 1)
            n += read(sd2, buf + n, sizeof(buf) - n);

          if (buf[len - 1] == '.')
            status = EXIT;
          else if (buf[len - 1] == '-')
            status = YOU;
          printf("%s: \n", buf);

        } while (status == PEER);
        break;
      }
    }

    close(sd2);
  }
}
