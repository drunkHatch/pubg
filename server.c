#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define	MY_PORT	2224

int	sock, snew, fromlength, number, outnum;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int grid_size;
int player_id = 0;
int random_seed;
void* server_loop(void *arg) {
		int position_x;
		int position_y;
		int position[2];
		printf("enter server_loop -----");
		send(sock,grid_size,1,0);   //send dimension of board
		send(sock,player_id,1,0);    //send id of player
		srand(random_seed);
		position_x = random(grid_size);
		position_y = random(grid_size);
		position[0] = position_x;
		position[1] = position_y;


		/*while (1) {
			recv(sock,tab,3,0);
			pthread_mutex_lock(&mutex);

			//update positon
			pthread_mutex_unlock(&mutex);
		}*/
    }

int main(int argc, char * argv[])
{
	struct	sockaddr_in	master, from;
	int client_sock;
	int i = 0;
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

	listen (sock, 5);
	fromlength = sizeof (from);

	while((client_sock = accept(sock, (struct sockaddr*) & from, & fromlength)) != -1)
    {
		printf("accept successfully!");
        pthread_create(&thread_id_client,NULL,server_loop,&client_sock);
		player_id++;
    }

	outnum = htonl (number);

	sleep(1);
	
}
