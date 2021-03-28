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

        char name[] = "shelbiee";;
        send(sockfd,name,8,0);
	int x;
	recv(sockfd,&x,4,0);
	int semid = semget(x,1,0666 | IPC_CREAT);
	int semval = semctl(semid,0,GETVAL);
	send(sockfd,&semval,4,0);
	close(sockfd);
	return 0;	
        /*int value;*/
       
	/*if (recv(sockfd, &value, 4, 0)!=4){
        	print_error("Error receiving integer.");	
        }       
	
	int same = value + 1;
	if (send(sockfd, &same, 4, 0)!=4){
		print_error("Error send increased value");
	}
	int arr[5];
	if (recv(sockfd, &arr, 20, 0)!=20){
		print_error("Error recieving array");
	}
	int j;
	int min = arr[0];
	for (j=0;j<5;j++){
		if (arr[j]<min){
			min = arr[j];
		}
	}
	int fl = min;
	printf("%d\n", fl);
	if (send(sockfd, &fl, 4, 0) != 4){
		print_error("Error sending smallest integer");
	}	
	struct box b1;
	if (recv(sockfd, &b1, 12, 0)!=12){
			print_error("error recieving volume");
	}
	int vol = b1.width * b1.height * b1.length;
	
	if (send(sockfd, &vol, 12, 0)!=12){
			print_error("error sending volume");
	}*/
}
void print_error(char *str) {
        printf("%s: %s\n", str, strerror(errno));
        exit(1);
}


// END OF FILE  
//
