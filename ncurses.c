#include <ncurses.h>

typedef struct _win_border_struct {
	chtype 	ls, rs, ts, bs, 
	 	tl, tr, bl, br;
}WIN_BORDER;

typedef struct _WIN_struct {

	int startx, starty;
	int height, width;
	WIN_BORDER border;
}WIN;

int player_x = 3;
int player_y = 5;
void init_win_params(WIN *p_win);
void print_win_params(WIN *p_win);
void create_box(WIN *win);
void create_player(bool flag,int direction);

int main(int argc, char *argv[])
{	WIN win;
	int ch;

	initscr();			/* Start curses mode 		*/
	curs_set(0);
	cbreak();			/* Line buffering disabled, Pass on
					 * everty thing to me 		*/
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/
	noecho();
	

	/* Initialize the window parameters */
	init_win_params(&win);
	refresh();
	
	create_box(&win);
	create_player(TRUE,3);
	int direction;
	while((ch = getch()))
	{	switch(ch)
		{	case KEY_LEFT:
				create_player(FALSE,1);
				--player_x;
				create_player(TRUE,1);
				break;
			case KEY_RIGHT:
				create_player(FALSE,2);
				++player_x;
				create_player(TRUE,2);
				break;
			case KEY_UP:
				create_player(FALSE,3);
				--player_y;
				create_player(TRUE,3);
				break;
			case KEY_DOWN:
				create_player(FALSE,4);
				++player_y;
				create_player(TRUE,4);
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
	{	mvaddch(player_y, player_x, '<');

	}
	if(flag == TRUE && direction == 2)
	{	mvaddch(player_y, player_x, '>');

	}
	if(flag == TRUE && direction == 3)
	{	mvaddch(player_y, player_x, '^');

	}
	if(flag == TRUE && direction == 4)
	{	mvaddch(player_y, player_x, 'v');

	}
	else
		mvaddch(player_y, player_x, ' ');
	refresh();
}