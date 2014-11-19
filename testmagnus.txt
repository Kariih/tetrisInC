#include <stdio.h>
#include <curses.h>

int main() {
	initscr();
	noecho();
	curs_set(FALSE);
	
	mvprintw(0,0,"hello world");
	sleep(3);

	endwin();
	return 0;
}
