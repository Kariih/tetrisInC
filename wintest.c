
#include <stdio.h>
#include <ncurses.h>

#define WIDTH 22
#define HEIGHT 22 

int startx = 0;
int starty = 0;

int main()
{	
	WINDOW *win;
	WINDOW *line;
	initscr();
	int x = 0, y = 0;
	int maxX = 0, maxY = 0;


	start_color();
	keypad(stdscr, TRUE);
	curs_set(FALSE);		
	startx = (40 - WIDTH) / 2;
	starty = (30 - HEIGHT) / 2;
	
	init_pair(1, COLOR_YELLOW, COLOR_YELLOW);
		
	win = newwin(HEIGHT, WIDTH, starty, startx);
	getmaxyx(win, maxX, maxY);	
	line = newwin(1, 8, 4, 4);
	wbkgd(line,COLOR_PAIR(1));
	while(1){	
		box(win, 0, 0);
		wrefresh(win);
		box(line, 0, 0);
		mvwin(line,y,x);
		wrefresh(line);
		nanosleep(100);

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
	getch();	
	endwin();
}

