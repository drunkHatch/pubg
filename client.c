
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

/* ---------------------------------------------------------------------
   This is a sample client program for the number server. The client and
   the server need to run on the same machine.
   --------------------------------------------------------------------- */

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int other_data = 1;
int init_sign = 0;
//https://stackoverflow.com/questions/14888027/mutex-lock-threads

int	sock, number;
char temp_data[11];
struct sockaddr_in server;
struct hostent *host;

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
		pthread_mutex_lock(&mutex);
		other_data = 2;
		temp_data[0] = 'a';
		printf("other_data: %d\n", other_data);
		pthread_mutex_unlock(&mutex);
		send(sock,temp_data,11,0);

	}
}

int main()
{
  pthread_t thread_key;
	int sign = 0;
	int rtv;

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

	bzero(temp_data,11);
	// receive init data
	temp_data[0] = 'A';
	temp_data[2] = 'B';

	send(sock,temp_data,11,0);
	//recv(sock,temp_data,7,0);

	/*******************************************************
	block for setting local data
	*******************************/

	if ((rtv = pthread_create(&thread_key, NULL, key_handler_thread, NULL))) {
		printf("%d\n", rtv);
		exit (1);
	}

	while (sign = recv(sock,temp_data,7,0)) {
		printf("recv success\n");
		pthread_mutex_lock(&mutex);
		printf("%s\n",temp_data);
		pthread_mutex_unlock(&mutex);

	}

	// Now we print out the character array to reveal that only 5 bytes were
	// received.
	close (sock);
}
