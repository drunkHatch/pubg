#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define	MY_PORT	2224


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



int main(int argc, char * argv[])
{
	int	sock, snew, fromlength, number, outnum;

	struct	sockaddr_in	master, from;

	int grid_size = argv[1];
	int i = 0;

	pthread_t thread_id_server, thread_id_client, thread_id_server_send;

	int client_sock;

    while((client_sock = accept(sock, (struct sockaddr*) & from, & fromlength)) != -1)
    {
        pthread_create(&thread_id_client,NULL,server_loop,&client_sock);
    }


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

	void send_init_players_pos() {
    	int i;
		send(sock,grid_size,1,0);   //send dimension of board
	
    	for (i = 0; i < MAX_PLAYERS; i++) {
			send(sock,i,1,0);    //send id of player
			srand(argv[4]);
			position_x = random(grid_size);
			position_y = random(grid_size);
			position = [position_x,position_y];
   	    }
    }

	void* server_loop(void *arg) {
		while (1) {
			recv(sock,tab,3,0);
			pthread_mutex_lock(&mutex);
		
			//update positon
			pthread_mutex_unlock(&mutex);
		}
	


    }

	listen (sock, 5);
	fromlength = sizeof (from);
	snew = accept (sock, (struct sockaddr*) & from, & fromlength);
	if (snew < 0) {
		perror ("Server: accept failed");
		exit (1);
	}
	outnum = htonl (number);
	
	sleep(1);
}