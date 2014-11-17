#include <ncurses.h>
#include <unistd.h>

#define delay 9000
#define WIDTH 10
#define HEIGHT 20 
#define BRICKWIDTH 10
#define BRICKHEIGHT 20 

struct brickData
{
	char brick[3][3];
	int color;
}; 

WINDOW *create_newwin(int height, int width, int starty, int startx);

int main() 
{
	int x = 0, y = 0;
	int maxY = 0, maxX = 0;
	int direction = 1;

	char line[3][3] = {{' ','#',' '},{' ','#',' '},{' ','#',' '}};
	char square[3][3] = {{' ',' ',' '},{' ','#','#'},{' ','#','#'}};
	char sBrick[3][3] = {{' ',' ',' '},{' ','#','#'},{'#','#',' '}};
	char zBrick[3][3] = {{' ',' ',' '},{'#','#',' '},{' ','#','#'}};
	char tBrick[3][3] = {{' ',' ',' '},{' ','#',' '},{'#','#','#'}};
	char LBrick[3][3] = {{'#','#',' '},{' ','#',' '},{' ','#',' '}};
	char bLBrick[3][3] = {{' ','#',' '},{' ','#',' '},{'#','#',' '}};

	init_pair(1, COLOR_CYAN, COLOR_CYAN);
	init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(3, COLOR_GREEN, COLOR_GREEN);
	init_pair(4, COLOR_RED, COLOR_RED);
	init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(6, COLOR_BLUE, COLOR_BLUE);
	init_pair(7, COLOR_WHITE, COLOR_WHITE);

	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);	
	maxX = WIDTH;
	maxY = HEIGHT;
	keypad(stdscr, TRUE);	
	nodelay(stdscr, TRUE);
	char turnX = 'j', turnY = 'i';

	char board[HEIGHT][WIDTH];
	x = 4;
	
	srand(time(NULL));
	int r = rand() % 7;

	/*struct brickData currentBrickArray[100];
	int g;
	for(g = 0; g <= 100; g++)
	{
		currentBrickArray[g].brick = tBrick;
		currentBrickArray[g].color = r;
	}*/
	
	int brickNumber = 0;

	while(1)
	{

		clear();
		int i , j, n;
		for (i = 0; i < HEIGHT; i++) 
		{
		printw("|");
			for (j = 0; j < WIDTH; j++) 
			{
				board[i][j] = ' ';
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
		refresh();
		
		attron(COLOR_PAIR(1)); //farge funker ikke.. must fix
		for (i = 0; i < 3; i++)
		{
			for(j = 0; j < 3; j++)
			{	
				
				board[i+y][j-2] = mvprintw(y+i, x+j,"%c", tBrick[i][j]);			

			}
		}

		/*if((y+BRICKHEIGHT) == HEIGHT)
		{
		x = 4;
		y = 0;
		brickNumber ++;		
		}
		else
		{
			y++;
		}*/
		y++;
		attroff(COLOR_PAIR(1));
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
			for (i = y; i < 3; i++)
			{
				for(j = x; j < 3; j++)
				{	
				
					board[i][x] = mvprintw(y+i, x+j,"%c", tBrick[i][j]);			

				}
			}
			refresh();
			break;
		case KEY_DOWN:  
			y++;
			break;
		}

	}
	endwin();
}