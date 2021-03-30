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
	
	int key;
	if (recv(sockfd,&key,4,0)!=4){
		print_error("Error receiving key");
	}
	
	char* board;
	if (recv(sockfd,&board,58,0)!=58){
		print_error("Error recieving String");
	}
	printf("%s");
	
	char* msg;
	if (recv(sockfd,&msg,17,0)!=17){
                print_error("Error recieving String");
        }
        printf("%s");

	int semid;
	struct sembuf op[1];
	int retval;
	op[0].sem_num = 0;
	op[0].sem_op = 1;
	op[0].sem_flg = 0;
	
	if ((retval = semop(semid, op, 1)) == -1)
		printf("Error incrementing semaphore", errno);
	int choice;
	scanf("%d", &choice);
	if (send(sockfd, &choice,4,0)!=4){
			print_error("Error sending choice");
		}
	/*int semid = semget(&key,1,0666 | IPC_CREAT);
	int semval = semctl(semid,0,GETVAL);
	int semid = semget(x,1,0666 | IPC_CREAT);
	int semval = semctl(semid,0,GETVAL);
	send(sockfd,&semval,4,0);
	close(sockfd);
	return 0;*/	
        /*int value;*/
       

}
void print_error(char *str) {
        printf("%s: %s\n", str, strerror(errno));
        exit(1);
}


// END OF FILE  
//
