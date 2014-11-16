#include <ncurses.h>
#include <unistd.h>

#define delay 9000
#define WIDTH 10
#define HEIGHT 20 

WINDOW *create_newwin(int height, int width, int starty, int startx);

int main() 
{
	int x = 0, y = 0;
	int max_y = 0, max_x = 0;
	int next_x = 0;
	int direction = 1;

	char line[3][3] = {{'0','#','0'},{'0','#','0'},{'0','#','0'}};
	char square[3][3] = {{'0','0','0'},{'0','#','#'},{'0','#','#'}};
	char sBrick[3][3] = {{'0','0','0'},{'0','#','#'},{'#','#','0'}};
	char zBrick[3][3] = {{'0','0','0'},{'#','#','0'},{'0','#','#'}};
	char tBrick[3][3] = {{'0','0','0'},{'0','#','0'},{'#','#','#'}};
	char LBrick[3][3] = {{'#','#','0'},{'0','#','0'},{'0','#','0'}};
	char bLBrick[3][3] = {{'0','#','0'},{'0','#','0'},{'#','#','0'}};

	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);	
	getmaxyx(stdscr, max_x, max_y);	
	keypad(stdscr, TRUE);	
	nodelay(stdscr, TRUE);

	char board[20][10];

	while(1)
	{

		clear();
		int i , j, n;
		for (i = 0; i < 20; i++) 
		{
		printw("|");
			for (j = 0; j < 10; j++) 
			{
				board[i][j] = '.';
				printw("%c ", board[i][j]);
			}	
		printw("|");
		printw("\n");
		}
		printw(" ");
		for (n = 0; n < 20; n++)
		{
			printw("_");
		}

		//mvprintw(y,x,"###");
		refresh();
		
		for (i = 0; i < 3; i++)
		{
			for(j = 0; j < 3; j++)
			{
				mvprintw(y, x,"%c", tBrick[i][j]);			

			}
			mvprintw(y,x,"\n");
		}
		refresh();
		x++;
		usleep(100);
	
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
