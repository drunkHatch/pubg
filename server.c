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

static void sig_handler(int signo){
    sigset_t signal_set;

    sigemptyset(&signal_set);

    if (signo == SIGINT){
        sigaddset(&signal_set, SIGALRM);
        sigprocmask(0, &signal_set, NULL);

        sigprocmask(1, &signal_set, NULL);
		/*******************************/
		// send signals to threads to cut connection
		exit(1);
		/*******************************/
    }
    else if (signo == SIGALRM){
        sigaddset(&signal_set, SIGINT);
        sigaddset(&signal_set, SIGUSR1);
        sigprocmask(0, &signal_set, NULL);
        system("date");
        dir = opendir(path);
        if (ENOENT == errno){
      //directory does not exist
          printf("Directory has been deleted!\n");
          exit(1);
        }
        dir_grabber(dir);
        closedir(dir);
        struct_move();
        reporter();
				signal (SIGALRM, sig_handler);
        alarm(period);
        sigprocmask(1, &signal_set, NULL);

    }
    else if (signo == SIGUSR1){
        sigaddset(&signal_set, SIGALRM);
        sigaddset(&signal_set, SIGINT);
        sigprocmask(0, &signal_set, NULL);
        system("date");
        dir = opendir(path);
        if (ENOENT == errno){
      //directory does not exist
          printf("Directory has been deleted!\n");
          exit(1);
        }
        dir_grabber(dir);
        closedir(dir);
        struct_move();
        reporter();
        signal(SIGUSR1, sig_handler);
        sigprocmask(1, &signal_set, NULL);

    }
}

void* server_loop(int *arg) {
		char imp[7];
		int local_socket;

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
