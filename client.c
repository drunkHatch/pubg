
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <pthread.h>

#define	MY_PORT 2224
#define SEND_LENGTH 100
#define RECV_LENGTH 100
#define INIT_LENGTH 100

/* ---------------------------------------------------------------------
   This is a sample client program for the number server. The client and
   the server need to run on the same machine.
   --------------------------------------------------------------------- */

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int other_data = 1;
int init_sign = 0;
//https://stackoverflow.com/questions/14888027/mutex-lock-threads

int	sock, number;
struct sockaddr_in server;
struct hostent *host;

char message_to_send[SEND_LENGTH];
char message_to_recv[RECV_LENGTH];
char message_init[INIT_LENGTH];

void* key_handler_thread(void* param)
{
	char key_pressed;

	printf("thread created\n");
	while (0) {
		switch(key_pressed)
		    {
		        case '+':
		            printf("+");
		            break;

		        case '-':
								printf("+");
		            break;

		        case '*':
								printf("+");
		            break;

		        case '/':
								printf("+");
		            break;

		        // operator is doesn't match any case constant (+, -, *, /)
		        default:
		            printf("ignore");
		    }

		printf("keyboard handler created\n");
		bzero(message_to_send, SEND_LENGTH);
		pthread_mutex_lock(&mutex);
		other_data = 2;
		message_to_send[0] = 'a';
		printf("other_data: %d\n", other_data);
		send(sock,message_to_send,SEND_LENGTH,0);
		pthread_mutex_unlock(&mutex);

	}
}

int main()
{
  	pthread_t thread_key;
	int sign = 0;
	int recv_rtv, thread_rtv;

	host = gethostbyname("localhost");

	if (host == NULL) {
		perror ("Client: cannot get host description");
		exit (1);
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0) {
		perror ("Client: cannot open socket");
		exit (1);
	}

	bzero(&server, sizeof (server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(MY_PORT);

	if (connect (sock, (struct sockaddr*) &server, sizeof (server))) {
		perror ("Client: cannot connect to server");
		exit (1);
	}

	if ((thread_rtv = pthread_create(&thread_key, NULL, key_handler_thread, NULL))) {
		perror("key thread create failed");
		exit (1);
	}

	bzero(message_to_recv,RECV_LENGTH);
	// receive init data
	recv(sock,message_init,INIT_LENGTH,0);
	// init game
	// init map
	// update backend info

	while ((recv_rtv = recv(sock,message_to_recv,RECV_LENGTH,0)) >= 0) {
		//do something to message_to_recv
		//recv commands first then go to different function to eat data

		// translate message to operations

		// init for next loop
		bzero(message_to_recv,RECV_LENGTH);
	}
	if (recv_rtv < 0) {
		perror("recv error\n");
		exit(1);
	}
	
	close (sock);
}
