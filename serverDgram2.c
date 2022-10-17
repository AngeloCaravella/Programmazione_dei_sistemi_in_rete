/*
* ServerDgram2.c
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
  int a;
  int b;
  long prod;
  /* The continuous loop characterizes the permanent execution of the server */
  while (1)
  {
    /* Clean the memory area that will store the data to send (buf) */
    memset(buf, 0, sizeof(buf));

    /* Receive the data from the client and put them in buf */
    recvfrom(serverSocket, buf, 2 * sizeof(buf), 0, (struct sockaddr *)&clientAddr, &clientAddrLen);

    a = *(int *)buf;
    b = *(int *)(buf + sizeof(int));
    prod = a * (long)b;

    sendto(serverSocket, &prod, sizeof(long), 0, (struct sockaddr *)&clientAddr, clientAddrLen);
  }
  return 0;
}
