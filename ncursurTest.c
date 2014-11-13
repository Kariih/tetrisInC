#include <ncurses.h>

int main ()
{
	initscr();
	raw();
	printw("Hello word");
	getch();
	endwin();



	return 0;
}
