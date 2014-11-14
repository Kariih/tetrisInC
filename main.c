#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
typedef struct
{
	int brick[3][3];
	int color;
}brickData; 

int main()
{	
	


	int line[3][3] = {{0,1,0},{0,1,0},{0,1,0}};
	int square[3][3] = {{0,0,0},{0,1,1},{0,1,1}};
	int sBrick[3][3] = {{0,0,0},{0,1,1},{1,1,0}};
	int zBrick[3][3] = {{0,0,0},{1,1,0},{0,1,1}};
	int tBrick[3][3] = {{0,0,0},{1,1,1},{1,1,1}};
	int LBrick[3][3] = {{1,1,0},{0,1,0},{0,1,0}};
	int bLBrick[3][3] = {{0,1,0},{0,1,0},{1,1,0}};
	
	init_pair(1, COLOR_CYAN, COLOR_CYAN);
	init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(3, COLOR_GREEN, COLOR_GREEN);
	init_pair(4, COLOR_RED, COLOR_RED);
	init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(6, COLOR_BLUE, COLOR_BLUE);
	init_pair(7, COLOR_WHITE, COLOR_WHITE);
	

	int board[20][10];
	int reachTop = 0;
	
	initscr();
	raw();
	
	printw("Tetris\n");
	int positionX, positionY;
	positionX = 4;
	while(reachTop != 20)
	{		
		printw("im here printw ");
		printf("im here printf");
		printw("\n");
			int i , j, n;
		    for (i = 0; i < 20; i++) {
			printw("|");
			for (j = 0; j < 10; j++) {
			    board[i][j] = '.';
			    printw("%c ", board[i][j]);
			}	
			printw("|");
			printw("\n");
		    }
			for (n = 0; n < 20; n++)
		{
		printw("_");
		}

	  	positionY++;
		
		printf("timer start");
		napms(1000);

		printf("timer end");
		refresh();
		reachTop++;
	}

	getch();
	endwin();
	return 0;
}
