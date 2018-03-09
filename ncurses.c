#include <ncurses.h>

#define INIT_DIRECTION 3

typedef struct _win_border_struct {
	chtype 	ls, rs, ts, bs,
	 	tl, tr, bl, br;
}WIN_BORDER;

typedef struct _WIN_struct {

	int startx, starty;
	int height, width;
	WIN_BORDER border;
}WIN;

int map[grid_size][grid_size] = {0};

int player_x = 3;
int player_y = 5;

void init_win_params(WIN *p_win);
void create_box(WIN *win);
void create_player(bool flag,int direction);
void create_bullet(WIN *p_win,bool flag,int direction);

int direction = INIT_DIRECTION;

int main(int argc, char *argv[])
{
	WIN win;
	WIN p_win;

	int ch;
	initscr();			/* Start curses mode 		*/
	curs_set(0);
	cbreak();			/* Line buffering disabled, Pass on
					 * everty thing to me 		*/
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/
	noecho();


	/* Initialize the window parameters */
	init_win_params(&p_win);
	refresh();

	create_box(&p_win);
	create_player(TRUE,3);

	while((ch = getch()))
	{	switch(ch)
		{	case 'j':
				create_bullet(&p_win,FALSE,direction);
				if(player_x > p_win.startx+1){
					direction = 1;
					create_player(FALSE,direction);
					--player_x;
					create_player(TRUE,direction);
				}
				break;
			case 'l':
				create_bullet(&p_win,FALSE,direction);
				if(player_x < p_win.startx+p_win.width-1 ){
					direction = 2;
					create_player(FALSE,direction);
					++player_x;
					create_player(TRUE,direction);
				}
				break;
			case 'i':
				create_bullet(&p_win,FALSE,direction);
				if(player_y > p_win.starty+1 ){
					direction = 3;
					create_player(FALSE,direction);
					--player_y;
					create_player(TRUE,direction);
				}
				break;
			case 'k':
				create_bullet(&p_win,FALSE,direction);
				if(player_y < p_win.starty+p_win.height-1 ){
					direction = 4;
					create_player(FALSE,direction);
					++player_y;
					create_player(TRUE,direction);
				}
				break;

			case ' ':
				create_bullet(&p_win,TRUE,direction);
				break;


		}
	}
	endwin();			/* End curses mode		  */
	return 0;
}



void init_win_params(WIN *p_win)
{
	p_win->height = 10;
	p_win->width = 10;
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
