#include <ncurses.h>
#include <unistd.h>

#define delay 9000
#define WIDTH 10
#define HEIGHT 20 
#define BRICKWIDTH 10
#define BRICKHEIGHT 20 

typedef struct
{
	char brick[3][3];
	int color;
} brickData; 

void gameLoop(char brick[3][3]);
brickData initGameBricks();

int main()
{
	brickData bricks;
	int maxX = 0, maxY = 0;
	maxX = WIDTH;
	maxY = HEIGHT;
	bricks = initGameBricks();
	
	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);
	keypad(stdscr, TRUE);	
	nodelay(stdscr, TRUE);

	char tBrick[3][3] = {{' ',' ',' '},{' ','#',' '},{'#','#','#'}};

	gameLoop(tBrick);
	endwin();
}

void gameLoop(char brick[3][3])
{

	int x = 0, y = 0;
	char board[HEIGHT][WIDTH];
	int brickNumber = 0;
	char turnX = 'j', turnY = 'i';

	x = 4;

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
				
				board[i+y][j-2] = mvprintw(y+i, x+j,"%c", brick[i][j]);			

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
				
					board[i][x] = mvprintw(y+i, x+j,"%c", brick[i][j]);			

				}
			}
			refresh();
			break;
		case KEY_DOWN:  
			y++;
			break;
		}
	}
}
brickData initGameBricks()
{
	brickData bricks;

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

	srand(time(NULL));
	int r = rand() % 7;

	/*struct brickData currentBrickArray[100];
	int i;
	for(i = 0; i <= 100; i++)
	{
		bricks[i].brick = tBrick;
		currentBrickArray[i].color = r;
	}*/

	return bricks;
}
