/*
 Modified on: October 13, 2022
 *      Author: Angelo Caravella

Write an application to implement the Tic-Tac-Toe game. The game is based on turns between two players who at each
turn place a pawn on a location of a 3 x 3 board. The first player that puts three pawns horizontally, vertically or on a
diagonal o the board is the winner. Implement the application by using the stream communication model based on TCP
sockets. If you use two processes for the game, their behavior should be symmetric, since for each turn they should be
able to perform the same actions. However, by using stream-oriented communication sockets, the initial interaction is
asymmetric: one player will run a process as server while the other one will run a process as client. When the connection
is established, the channel can be used in symmetric way.*/
/*#include <sys/types.h>*/

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
#define OTHER 1

int check_winner();
void print_board();

/* global variables used for the application state */
char board[10] = {0, '1', '2', '3', '4', '5', '6', '7', '8', '9'};
char mark;

int main(int argc, char *argv[])
{
    struct sockaddr_in sad; /* Struct to store the transport address of the server socket */
    struct sockaddr_in cad; /* Struct to store the transport address of the client socket */
    int sd, sd2;            /* Socket descriptors */
    int port;               /* Port number */
    socklen_t alen;         /* Lenght of the client transport address  */
    char buf[1000];         /* Buffer for sending and receiving data */
    int visits = 0;         /* Status variable to count the number of connections from clients */
    int n = 0;
    int len;
    int player;
    int terminated, choice;
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

    /* Extract a connection request from the queue and create a new socket */
    if ((sd2 = accept(sd, (struct sockaddr *)&cad, &alen)) < 0)
    {
        fprintf(stderr, "accept failed\n");
        exit(1);
    }
    player = OTHER;

    do
    {
        print_board();
        if (player == OTHER)
        {
            read(sd2, &choice, sizeof(int));
            printf("Choice of the other player: %d\n", choice);
        }
        else
        {
            printf("Enter your choice:");
            scanf("%d", &choice);
            write(sd2, &choice, sizeof(int));
        }
        mark = (player == 0) ? 'X' : 'O';
        if (board[choice] >= '1' && board[choice] <= '9')
            board[choice] = mark;
        else
            printf("Invalid choice \n");
        terminated = check_winner();
        player = !player;
    } while (!terminated);


print_board();
printf("Player %d wins \n\n", !player + 1);
close(sd);
exit(0);

/* Close the connection socket created by accept() */
close(sd2);
}

void print_board()
{
    printf("\n\n\tTic Tac Toe\n\n");
    printf("Player 1 (X) - Player 2 (O)\n\n\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c \n", board[1], board[2], board[3]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c \n", board[4], board[5], board[6]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c \n", board[7], board[8], board[9]);
    printf("     |     |     \n\n");
}

int check_winner()
{

    for (int i = 1; i <= 7; i += 3)
        if (board[i] == board[i + 1] && board[i + 1] == board[i + 2])
            return 1;
    for (int i = 1; i <= 3; i++)
        if (board[i] == board[i + 3] && board[i + 3] == board[i + 6])
            return 1;
    if (board[1] == board[5] && board[5] == board[9])
        return 1;

    if (board[3] == board[5] && board[5] == board[7])
        return 1;

    return 0;
}
