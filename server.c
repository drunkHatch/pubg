#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <ncurses.h>

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> //optional
#include <sys/inotify.h>
#include <inttypes.h>
#include <limits.h>
#include <errno.h>
#include <sys/time.h>



#define ID_SIZE (sizeof(TRACKER))
#define SEND_LENGTH 100
#define RECV_LENGTH 100
#define INIT_LENGTH 100


typedef struct _win_border_struct {
	chtype 	ls, rs, ts, bs,
	 	tl, tr, bl, br;
}WIN_BORDER;

typedef struct _WIN_struct {

	int startx, starty;
	int height, width;
	WIN_BORDER border;
}WIN;


//int sock, snew, fromlength, number, outnum;
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
char *raw_grid_size;
int player_id = 0;
int random_seed;
float interval = 0.2;
int **map;
//int map[grid_size][grid_size];

pthread_mutex_t mutex_thread_array = PTHREAD_MUTEX_INITIALIZER;
TRACKER *thread_array;
int thread_length = 0;
int connection_count = 0;

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

		/*******************************/
		// send signals to threads to cut connection
		exit(1);
		/*******************************/
    }
    else if (signo == SIGALRM){
        sigaddset(&signal_set, SIGTERM);
        sigprocmask(0, &signal_set, NULL);

		// send SIGUSR1 to all threads to send data to all clients
		signal(SIGALRM, sig_handler);
        alarm(interval);
        sigprocmask(1, &signal_set, NULL);
    }
	else{
		perror("incorrect signal received by thread");
		exit(1);
	}
}

int random_func(){
	int rtv, temp;

	temp = rand();
	rtv = temp % grid_size;
	return rtv;
}

char *init_message_prep(){

}

int calculate_key(){




}

void* server_loop(int *arg) {

		char message_init[INIT_LENGTH];
		char message_to_recv[RECV_LENGTH];
		char message_to_send[SEND_LENGTH];
		int local_socket;
		int thread_place;
		int recv_rtv;
		int size_length;
		int local_id = 0;
		int player_x, player_y;
		TRACKER this_thread;
		sigset_t signal_set;

		// critical section initialization
		printf("thread created lollololol\n");
		local_socket = *arg; // store socket info to local var
		thread_length++; // extend thread length
		connection_count++;
		this_thread.connection_number = connection_count;
		this_thread.current_id = pthread_self(); // get current thread id
		thread_array = (TRACKER *)realloc(thread_array, thread_length * ID_SIZE); // alloc places for new id
		thread_array[thread_length - 1] = this_thread; // add new thread to records

		pthread_mutex_unlock(&mutex_socket);
		pthread_mutex_unlock(&mutex_thread_array);
		// critical section ends

		// add sending signal(SIGUSR1) to signal_set
		sigemptyset(&signal_set);
		sigaddset(&signal_set, SIGUSR1);

		// store value in message
		// grid_size
		size_length = strlen(raw_grid_size);
		// send length
		// id
		player_id++;
		local_id = player_id;
		// location
		while(1){
			player_x = random_func();
			player_y = random_func();
			if (map[player_x][player_y] == 0){
				break;
			}
		}
		bzero(message_to_send,INIT_LENGTH);
		send(local_socket,raw_grid_size,INIT_LENGTH,0);   //send init data
		bzero(message_to_send,10);
		sprintf(message_to_send, "%d", local_id);
		send(local_socket,message_to_send,10,0);   //send init data
		bzero(message_to_send,5);
		sprintf(message_to_send, "%d", player_x);
		send(local_socket,message_to_send,5,0);   //send init data
		bzero(message_to_send,5);
		sprintf(message_to_send, "%d", player_y);
		send(local_socket,message_to_send,5,0);   //send init data
		bzero(message_to_send,INIT_LENGTH);
		//send(local_socket,message_init,INIT_LENGTH,0);   //send init data

		bzero(message_to_recv,1);
		printf("pass\n");		
		while ((recv_rtv = recv(local_socket,message_to_recv,1,0)) >= 0) {
			//do something to raw_data
			printf("%d\n", recv_rtv);
			printf("%c\n", message_to_recv[0]);


			// wait for sending signal (SIGUSR1)
			bzero(message_to_recv,1);
			//sigsuspend(&signal_set);
			//send(local_socket,raw_data,7,0); //send data to client here
		}

		if (recv_rtv < 0) {
			perror("recv error\n");
			exit(1);
		}

		/*
		bzero(raw_data,7);
		raw_data[0] = 'M';
		raw_data[1] = 'M';
		raw_data[2] = 'P';
		raw_data[3] = 'D';
		*/
		close(local_socket);

    }

int main(int argc, char * argv[])
{
	struct	sockaddr_in	master, from;
	int client_sock, copy_client;
	int i = 0, temp, fromlength;
	int m = 0;
	pthread_t thread_id_server, thread_id_client, thread_id_server_send;

	signal(SIGALRM, sig_handler);
	signal(SIGTERM, sig_handler);

	thread_array = (TRACKER *)malloc(0);
	random_seed = atol(argv[4]);
	grid_size = atoi(argv[1]);
	raw_grid_size = argv[1];
	srand(random_seed);

	map = malloc(sizeof(int *) * grid_size);
	for (m = 0; m < grid_size; m++){
        map[m] = malloc(sizeof(int) * grid_size);
    }
	for (size_t i = 0; i < grid_size; i++) {
		for (size_t j = 0; j < grid_size; j++) {
			map[i][j] = 0;
		}
	}
	sock = socket (AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror ("Server: cannot open master socket");
		exit (1);
	}

	master.sin_family = AF_INET;
	master.sin_addr.s_addr = inet_addr("127.0.0.1");
	master.sin_port = htons (atoi(argv[3]));

	if (bind (sock, (struct sockaddr*) &master, sizeof (master))) {
		perror ("Server: cannot bind master socket");
		exit (1);
	}

	listen(sock, 100);
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
