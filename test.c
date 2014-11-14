#include <ncurses.h>
#include <unistd.h>

#define delay 9000
#define WIDTH 10
#define HEIGHT 20 

int main() 
{
	WINDOW *win;
	int x = 0, y = 0;
	int max_y = 0, max_x = 0;
	int next_x = 0;
	int direction = 1;

	initscr();
	//noecho();
	cbreak();
	curs_set(FALSE);	
	getmaxyx(stdscr, max_x, max_y);	
	keypad(stdscr, TRUE);
	
	nodelay(stdscr, TRUE);
	while(1)
	{

		clear();
		mvprintw(y,x,"o");
		refresh();
		sleep(1);
	
		int ch = mvgetch(x, y);
     		switch (ch) {
		case KEY_LEFT: 
			x--;
			break;
		case KEY_RIGHT: 
			x++;
			break;
		case KEY_UP:  
   			y--;
			break;
		case KEY_DOWN:  
			y++;
			break;
		}

	}
	endwin();
}
