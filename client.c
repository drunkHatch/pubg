
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
#include <ncurses.h>

#define SEND_LENGTH 100
#define RECV_LENGTH 100
#define INIT_LENGTH 100
#define INIT_DIRECTION 3

// two structs are UI
typedef struct _win_border_struct {
	chtype 	ls, rs, ts, bs,
	 	tl, tr, bl, br;
}WIN_BORDER;

typedef struct _WIN_struct {

	int startx, starty;
	int height, width;
	WIN_BORDER border;
}WIN;

// player postion
int player_x = 0;
int player_y = 0;

void init_win_params(WIN *p_win, int size); // windows parameters
void create_box(WIN *win); // print window
void create_player(bool flag,int direction); //print player
void create_bullet(WIN *p_win,bool flag,int direction); // print bullet

int direction = INIT_DIRECTION;

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

WIN p_win;

void create_bullet(WIN *p_win,bool flag,int direction){
	if(flag == TRUE){
		switch(direction)
		{	case 1:
				if(player_x > p_win->startx+2){
					attron(A_BOLD);
					mvaddch(player_y, player_x-1, 'o');
					mvaddch(player_y, player_x-2, 'o');
					attroff(A_BOLD);
				}
				else if(player_x == p_win->startx+2){
					attron(A_BOLD);
					mvaddch(player_y, player_x-1, 'o');
					attroff(A_BOLD);
				}

				break;

			case 2:
				if(player_x < p_win->startx + p_win->width-2){
					attron(A_BOLD);
					mvaddch(player_y, player_x+1, 'o');
					mvaddch(player_y, player_x+2, 'o');
					attroff(A_BOLD);
				}
				else if(player_x == (p_win->startx + p_win->width-2)){
					attron(A_BOLD);
					mvaddch(player_y, player_x+1, 'o');
					attroff(A_BOLD);
				}

				break;

			case 3:
				if(player_y > p_win->starty+2){
					attron(A_BOLD);
					mvaddch(player_y-1, player_x, 'o');
					mvaddch(player_y-2, player_x, 'o');
					attroff(A_BOLD);
				}
				else if(player_y == p_win->starty+2){
					attron(A_BOLD);
					mvaddch(player_y-1, player_x, 'o');
					attroff(A_BOLD);
				}
				break;

			case 4:
				if(player_y < p_win->starty+p_win->height-2){
					attron(A_BOLD);
					mvaddch(player_y+1, player_x, 'o');

					mvaddch(player_y+2, player_x, 'o');
					attroff(A_BOLD);
				}
				else if(player_y == p_win->starty+p_win->height-2){
					attron(A_BOLD);
					mvaddch(player_y+1, player_x, 'o');
					attroff(A_BOLD);
				}
				break;}

	}

	else {
		switch(direction)
		{	case 1:
				mvaddch(player_y, player_x-1, ' ');
				mvaddch(player_y, player_x-2, ' ');
				break;

			case 2:
				mvaddch(player_y, player_x+1, ' ');
				mvaddch(player_y, player_x+2, ' ');
				break;

			case 3:
				mvaddch(player_y-1, player_x, ' ');
				mvaddch(player_y-2, player_x, ' ');
				break;
			case 4:
				mvaddch(player_y+1, player_x, ' ');
				mvaddch(player_y+2, player_x, ' ');
				break;
	    }
	}

	refresh();
	}


// success
void init_win_params(WIN *p_win, int size)
{
	p_win->height = size;
	p_win->width = size;
	p_win->starty = 1;
	p_win->startx = 1;

	p_win->border.ls = '|';
	p_win->border.rs = '|';
	p_win->border.ts = '-';
	p_win->border.bs = '-';
	p_win->border.tl = '+';
	p_win->border.tr = '+';
	p_win->border.bl = '+';
	p_win->border.br = '+';

}

// success
void create_player(bool flag,int direction){

	if(flag == TRUE && direction == 1)
	{	mvaddch(player_y, player_x, '<');}

	else if(flag == TRUE && direction == 2)
	{	mvaddch(player_y, player_x, '>');}

	else if(flag == TRUE && direction == 3)
	{	mvaddch(player_y, player_x, '^');}


	else if(flag == TRUE && direction == 4)
	{	mvaddch(player_y, player_x, 'v');}


	else{
		mvaddch(player_y, player_x, ' ');
	}

	refresh();
}

// success
void create_box(WIN *p_win)
{	int i, j;
	int x, y, w, h;

	x = p_win->startx;
	y = p_win->starty;
	w = p_win->width;
	h = p_win->height;


	mvaddch(y, x, p_win->border.tl);
	mvaddch(y, x + w, p_win->border.tr);
	mvaddch(y + h, x, p_win->border.bl);
	mvaddch(y + h, x + w, p_win->border.br);
	mvhline(y, x + 1, p_win->border.ts, w - 1);
	mvhline(y + h, x + 1, p_win->border.bs, w - 1);
	mvvline(y + 1, x, p_win->border.ls, h - 1);
	mvvline(y + 1, x + w, p_win->border.rs, h - 1);

	refresh();
}


void* key_handler_thread(WIN *p_win )
{
	int ch;
	char sender[2];

	printf("thread created\n");
	while((ch = getch()))
	{	switch(ch)
		{	case 'j':
				send(sock,"j",1,0);   //send init data
				perror ("Client: entered thread");
				exit (1);
				break;
			case 'l':
				send(sock,"l",1,0);   //send init data
				printw("info");

				break;
			case 'i':
				send(sock,"i",1,0);   //send init data
				printw("info");

				break;
			case 'k':
				send(sock,"k",1,0);   //send init data
				printw("info");

				break;
			case 'x':
				send(sock,"x",1,0);   //send init data
				printw("info");

				break;
			case ' ':
				send(sock,' ',1,0);   //send init data
				printw("info");

				break;
		}
	}
}

int main(int argc, char *argv[])
{
  	pthread_t thread_key;
	int sign = 0;
	int recv_rtv, thread_rtv;
	int key;	// input variable

	// variables from init message
	int grid_size;
	int id;

	 // window variable

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
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_port = htons(atoi(argv[2]));

	if (connect (sock, (struct sockaddr*) &server, sizeof (server))) {
		perror ("Client: cannot connect to server");
		exit (1);
	}


	if ((thread_rtv = pthread_create(&thread_key, NULL, key_handler_thread, &p_win))) {
		perror("key thread create failed");
		exit (1);
	}

	bzero(message_to_recv,RECV_LENGTH);
	// receive init data
	recv(sock,message_init,INIT_LENGTH,0);
	grid_size = atoi(message_init);
	bzero(message_init,10);
	recv(sock,message_init,10,0);
	id = atoi(message_init);
	bzero(message_to_send,5);
	recv(sock,message_init,5,0);
	player_x = atoi(message_init);
	bzero(message_init,5);
	recv(sock,message_init,5,0);
	player_y = atoi(message_init);
	// update backend info


	p_win.startx = 1;
	p_win.starty = 1;
	p_win.width = grid_size + 1;
	p_win.height = grid_size + 1;

	// init game
	/*******************************/
	// initialization of window
	initscr();			/* Start curses mode 		*/
	curs_set(0);	// hide mouse
	cbreak();			/* Line buffering disabled, Pass on
					 * everty thing to me 		*/
	keypad(stdscr, TRUE);		/* turn on key input	*/
	noecho(); // ??
	/*******************************/
	// init map
	/* Initialize the window parameters */
	init_win_params(&p_win, (grid_size + 1));
	refresh();
	/*******************************/
	// print window
	create_box(&p_win);
	create_player(TRUE,3);
	/*******************************/



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
