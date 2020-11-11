/* Nguyen Thanh Nam - 18020925 */
/* Chia ra thanh cac trang thai voi cac input
 * socket(), bind(), connent() duoc dua tren code cua thay Son */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include  <unistd.h>
#include <ctype.h>	


#define SERVER_PORT 9000
#define BUFFER_SIZE 1024

/* use enum for state of socket */
enum stateType {LISTEN, W_COMMAND, W_NAME, W_AGE}; 

int main(int argc, char **argv) {
    int     listenPort = SERVER_PORT;
    int 	servSock, cliSock;
    struct sockaddr_in servSockAddr, cliAddr;
    int 	servSockLen, cliAddrLen;
    char 	recvBuf[BUFFER_SIZE];
    int 	n;
    enum stateType state = LISTEN;
    char temp[BUFFER_SIZE]; // copy recvBuf to use
    
    if (argc >= 2){
	listenPort = atoi(argv[1]);
    }
    servSock = socket(AF_INET, SOCK_STREAM,0);
    bzero(&servSockAddr, sizeof(servSockAddr));
    servSockAddr.sin_family = AF_INET;
    servSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servSockAddr.sin_port = htons(listenPort);
    
    servSockLen = sizeof(servSockAddr);
    if(bind(servSock,(struct sockaddr*)&servSockAddr, servSockLen) < 0) {
	perror("bind"); exit(1);
    }
    if(listen(servSock, 5) < 0) {
	perror("listen");
	exit(1);
    }
    while(1) {
	    printf("Waiting for client...\n");
	    cliSock = accept(servSock, (struct sockaddr *) &cliAddr, (socklen_t *) &cliAddrLen);
	    printf("Received a connection from client %s\n", inet_ntoa(cliAddr.sin_addr));
	    while(1) {
		    n = read(cliSock, recvBuf, BUFFER_SIZE-1);
		    
		    // asign termination
		    recvBuf[n] = '\0';
		    if (n == 0 || (strcmp(recvBuf, "QUIT") == 0)) {
				printf("500 bye. \nClose the connection.\n");
				close(cliSock);
				break;
		    }
		    printf("Received a message from the client:%s\n", recvBuf);
		    if (state == LISTEN) {
			    if (strcmp(recvBuf, "HELO Server") == 0) {
				    state = W_COMMAND;
				    strcpy(recvBuf, "200 Hello Client");
			    } else {
				    strcpy(recvBuf, "Invalid command, try HELO Server");
			    }
		    }

		    else if (state == W_COMMAND) {
			    if (strcmp(recvBuf, "USER NAME") == 0) {
				    state = W_NAME;
				    strcpy(recvBuf, "210 User name OK");
			    } else if (strcmp(recvBuf, "USER AGE") == 0) {
				    state = W_AGE;
				    strcpy(recvBuf, "220 User age OK");
			    } else if (strcmp(recvBuf, "QUIT") == 0) {
				    state = LISTEN;
				    strcpy(recvBuf, "500 bye");
			    } else {
				    strcpy(recvBuf, "Invalid command");
			    }
		    }

		    // user name input
		    else if (state == W_NAME) {
			    int check = 1; // check command
			    
			    if (check) {
				    state = W_COMMAND;
				    
			    } else {
				    strcpy(recvBuf, "410 User name error");
			    }
			// user age input
		    } else if (state == W_AGE) {
			    int check = 1;
			    for (int i = 0; i < n; i++) {
				    if(!isdigit(recvBuf[i])) {
					    check = 0;
					    break;
				    }
			    }
			    if (check) {
				    state = W_COMMAND;
				    strcpy(temp, recvBuf);
				    sprintf(recvBuf, "221 User age %s OK", temp);
			    } else {
				    strcpy(recvBuf, "420 User age error");
			    }
		    }

		    write(cliSock, recvBuf, strlen(recvBuf));
		    printf("Sent a message to the client: %s\n", recvBuf);
	    }
	}
}



			
