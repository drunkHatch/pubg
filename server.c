#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#define	MY_PORT	2224
#define ID_SIZE (sizeof(TRACKER))

//int	sock, snew, fromlength, number, outnum;
typedef struct{
	int id;
	int kills;
} PLAYER;

typedef struct{
	int connection_number;
	pthread_t current_id;
} TRACKER;

pthread_mutex_t mutex_socket = PTHREAD_MUTEX_INITIALIZER;
int	sock, snew, number;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int grid_size;
int player_id = 0;
int random_seed;
float interval = 0.2;

pthread_mutex_t mutex_thread_array = PTHREAD_MUTEX_INITIALIZER;
TRACKER *thread_array;
int thread_length = 0;
int connection_count = 0;


int get_tid_index(pthread_t item, pthread_t *t_array){

}

static void thread_sig_handler(int signo){
	/*
	how many signals
	sigterm for daemon: shut down properly
	sigalarm for all threads: get signal then send updated data

	*/
	if (signo == SIGUSR1){
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
        signal(SIGUSR1, thread_sig_handler);
        sigprocmask(1, &signal_set, NULL);
    }
	else{
		perror("incorrect signal received by thread");
		exit(1);
	}
}

static void sig_handler(int signo){
	/*
	how many signals
	sigterm for daemon: shut down properly
	sigalarm for all threads: get signal then send updated data

	*/
    sigset_t signal_set;

    sigemptyset(&signal_set);

    if (signo == SIGTERM){
        sigaddset(&signal_set, SIGALRM);
        sigprocmask(0, &signal_set, NULL);

        sigprocmask(1, &signal_set, NULL);
		/*******************************/
		// send signals to threads to cut connection
		exit(1);
		/*******************************/
    }
    else if (signo == SIGALRM){
        sigaddset(&signal_set, SIGTERM);
        sigprocmask(0, &signal_set, NULL);

		// send signal to all threads to send data to all clients
		signal(SIGALRM, sig_handler);
        alarm(interval);
        sigprocmask(1, &signal_set, NULL);

    }
}

void* server_loop(int *arg) {
		char imp[7];
		int local_socket;
		int thread_place;
		TRACKER this_thread;

		local_socket = *arg; // store socket info to local var
		thread_length++; // extend thread length
		connection_count++;
		this_thread.connection_number = connection_count;
		this_thread.current_id = pthread_self(); // get current thread id
		thread_array = (TRACKER *)realloc(thread_array, thread_length * ID_SIZE); // alloc places for new id
		thread_array[thread_length - 1] = this_thread; // add new thread to records

		pthread_mutex_unlock(&mutex_socket);
		pthread_mutex_unlock(&mutex_thread_array);

		bzero(imp,7);
		imp[0] = 'M';
		imp[1] = 'M';
		imp[2] = 'P';
		imp[3] = 'D';

		send(local_socket,imp,7,0);   //send dimension of board

		close(local_socket);
    }

int main(int argc, char * argv[])
{
	struct	sockaddr_in	master, from;
	int client_sock, copy_client;
	int i = 0, temp, fromlength;
	pthread_t thread_id_server, thread_id_client, thread_id_server_send;

	signal(SIGALRM, sig_handler);
	signal(SIGTERM, sig_handler);

	thread_array = (TRACKER *)malloc(0);
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
	alarm(interval);
	while((client_sock = accept(sock, (struct sockaddr*) &from, &fromlength)) != -1)
    {

		printf("accept successfully!\n");

		pthread_mutex_lock(&mutex_socket);
		pthread_mutex_lock(&mutex_thread_array);
		copy_client = client_sock;

		temp = pthread_create(&thread_id_client,NULL,server_loop,&copy_client);

		printf("new client connected successfully\n");
		if (temp != 0) {
			perror ("Server: client thread creation failed\n");
			exit (1);
		}
    }
	sleep(1);
	printf("reach EOF\n");
	//close(client_sock);
}
