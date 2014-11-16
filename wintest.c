
#include <stdio.h>
#include <ncurses.h>

#define WIDTH 10
#define HEIGHT 20 

int startx = 0;
int starty = 0;

int main()
{	
	WINDOW *win;
	WINDOW *ine;
	initscr();



	startx = (80 - WIDTH) / 2;
	starty = (24 - HEIGHT) / 2;
		
	win = newwin(HEIGHT, WIDTH, starty, startx);
	while(1){	
		box(win, 0, 0);
		wrefresh(win);
		sleep(1);
	}
	getch();	
	endwin();
}

