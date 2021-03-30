#include <netdb.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


extern int errno;
static int sockfd;

void print_error(char *);

int main(int argc, char *argv[]) {
        struct addrinfo *host_ai;
        struct addrinfo hint;
        int err;

        if (argc != 2) {
                print_error("usage: client hostname");
    }

        hint.ai_flags = 0;
        hint.ai_family = AF_INET;
        hint.ai_socktype = SOCK_STREAM;
        hint.ai_protocol = 0;
        hint.ai_addrlen = 0;
        hint.ai_canonname = NULL;
        hint.ai_addr = NULL;
        hint.ai_next = NULL;

        if ((err = getaddrinfo(argv[1], "tokenserver", &hint, &host_ai)) != 0) {
                printf("getaddrinfo error: %s\n", gai_strerror(err));
                return 0;
        }

        printf("creating socket\n");
        if ((sockfd = socket(host_ai->ai_addr->sa_family, SOCK_STREAM, 0)) < 0) {
                print_error("Error creating socket");
        }
        printf("socket created\n");

        printf("attempting Connection\n");
        if (connect(sockfd, host_ai->ai_addr, host_ai->ai_addrlen) != 0) {
                close(sockfd);
                printf("can't connect to %s\n", argv[1]);
                print_error("Error connecting to server");
        }
        printf("connection made...\n");
        freeaddrinfo(host_ai);
/*The client receives a key from the server
 * then sets the semID*/	
	int key;
	if (recv(sockfd,&key,4,0)!=4){
		print_error("Error receiving key");
	}
	int semid;
	if((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1){
			print_error("Error getting id");
	}
/*The client receives the tic-tac-toe board
 * Then prints the board to the client*/
	char* board;
	if (recv(sockfd,&board,58,0)!=58){
		print_error("Error recieving board");
	}

	printf("%s");
        /*struct sembuf op[1];
        int retval;*/
	int choice;
	char msg[17];
	char status[12];
	_Bool win = 0;
/*Making a while loop for the gameplay and ends when the game is over*/
	while(win!=1){
/*receiving the message from server on whos turn it is
 * Then prints it*/
	if (recv(sockfd,&msg,17,0)!=17){
                print_error("Error recieving message");
        }
        printf("%s", &msg);
/*Comparing a string to tell whos turn it is
 * and then asking the client for their input on where they would want to go for their turn*/
	if (strcmp(msg, "It is your turn\n'\0'")==0){
		scanf("%d",&choice);
		if (send(sockfd, &choice,4,0)!=4){
                        print_error("Error sending choice");
        	}
		/*op[0].sem_num = 0;
        	op[0].sem_op = 1;
        	op[0].sem_flg = 0;
		if ((retval = semop(semid, op, 1)) == -1){
                	print_error("Error incrementing semaphore");
        	}*/
	}
/*This then receives the game board in the loop to keep the clients updated on the board*/
	if (recv(sockfd,&board,58,0)!=58){
                print_error("Error recieving updated board");
        }
                printf("%s",&board);
/*This receives the boolean from the server to check of someone has won and and exits the game if someone has*/
		if(recv(sockfd, &win, 1, 0)!=1){
			print_error("error receiving boolean");
		}
	}
/*Once out of the while loop it receives the game status message from the server and prints it*/
      if (recv(sockfd,&status,12,0)!=12){
	      print_error("Error receiveing game status");
	}
      printf("%s", &status);

}
void print_error(char *str) {
        printf("%s: %s\n", str, strerror(errno));
        exit(1);
}


// END OF FILE  
//
