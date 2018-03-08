#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define	MY_PORT	2224

//int	sock, snew, fromlength, number, outnum;
int	sock, snew, number;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_socket = PTHREAD_MUTEX_INITIALIZER;

int grid_size;
int player_id = 0;
int random_seed;

void* server_loop(int *arg) {
		char imp[7];
		int local_socket;
		/*
		int position_x;
		int position_y;
		int position[2];
		*/
		local_socket = *arg;
		pthread_mutex_unlock(&mutex_socket);
		printf("enter server_loop\n");

		bzero(imp,7);
		imp[0] = 'M';
		imp[1] = 'M';
		imp[2] = 'P';
		imp[3] = 'D';

		send(local_socket,imp,7,0);   //send dimension of board
		printf("%d\n", local_socket);
		//send(sock,player_id,1,0);    //send id of player
		//srand(random_seed);

		/*
		position_x = rand() % grid_size;
		position_y = rand() % grid_size;
		printf("%d\n", position_x);
		position[0] = position_x;
		position[1] = position_y;
		*/
		/*
		while (1) {
			recv(sock,tab,3,0);
			pthread_mutex_lock(&mutex);

			//update positon
			pthread_mutex_unlock(&mutex);
		}
		*/
		close(local_socket);
    }

int main(int argc, char * argv[])
{
	struct	sockaddr_in	master, from;
	int client_sock, copy_client;
	int i = 0, temp, fromlength;
	pthread_t thread_id_server, thread_id_client, thread_id_server_send;


	random_seed = atol(argv[4]);
	grid_size = atoi(argv[1]);

	sock = socket (AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror ("Server: cannot open master socket");
		exit (1);
	}

	master.sin_family = AF_INET;
	master.sin_addr.s_addr = inet_addr("127.0.0.1");
	master.sin_port = htons (MY_PORT);

	if (bind (sock, (struct sockaddr*) &master, sizeof (master))) {
		perror ("Server: cannot bind master socket");
		exit (1);
	}

	listen(sock, 5);
	fromlength = sizeof (from);

	//outnum = htonl (number);

	//update positon
	while((client_sock = accept(sock, (struct sockaddr*) &from, &fromlength)) != -1)
    {

		printf("accept successfully!\n");

		pthread_mutex_lock(&mutex_socket);
		copy_client = client_sock;

		temp = pthread_create(&thread_id_client,NULL,server_loop,&copy_client);

		printf("new client connected successfully\n");
		if (temp != 0) {
			perror ("Server: client thread creation failed\n");
			exit (1);
		}
		player_id++;
    }
	sleep(1);
	printf("reach EOF\n");
	//close(client_sock);
}
