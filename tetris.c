#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <vlc/vlc.h>

#define WIDTH 10
#define HEIGHT 20
#define BLOCK_WIDTH 4
#define BLOCK_HEIGHT 4

//set up structs and variables
typedef unsigned int Byte;
typedef unsigned char Color;

typedef struct 
{
	int x;
	int y;
} Position;

typedef struct 
{
	Byte blocks[BLOCK_WIDTH][BLOCK_HEIGHT];
	Color color;
	Position position;
} Block;

typedef Byte Map[HEIGHT][WIDTH];

Block block;
Block nextblock;
Block holdblock;

libvlc_instance_t *inst;
libvlc_media_player_t *mediaplayer;
libvlc_media_t *media;

Map map;

int delay = 500;
int hold = 0;
int stoploop = 1;
int pauseKey = 0;
int point;
int isPlaying = 0;
int times;
int game = 1;

//Set blocks and colors
void newBlock(Block * block) 
{
	memset(block, 0, sizeof(Block));		

	int blockId = rand() % 7;

	switch(blockId) {
		case 0:
			//T block
			block->blocks[1][0] = 1;
			block->blocks[1][1] = 1;
			block->blocks[1][2] = 1;
			block->blocks[0][1] = 1;			
		break;
		case 1:
			//Cube
			block->blocks[0][0] = 1;
			block->blocks[0][1] = 1;
			block->blocks[1][0] = 1;
			block->blocks[1][1] = 1;			
		break;
		case 2:
			//J block
			block->blocks[0][0] = 1;
			block->blocks[0][1] = 1;
			block->blocks[0][2] = 1;
			block->blocks[1][2] = 1;			
		break;
		case 3:
			//L block
			block->blocks[0][2] = 1;
			block->blocks[1][0] = 1;
			block->blocks[1][1] = 1;
			block->blocks[1][2] = 1;			
		break;
		case 4:
			//Z block
			block->blocks[0][0] = 1;
			block->blocks[0][1] = 1;
			block->blocks[1][1] = 1;
			block->blocks[1][2] = 1;			
		break;
		case 5:
			//S block
			block->blocks[1][0] = 1;
			block->blocks[1][1] = 1;
			block->blocks[0][1] = 1;
			block->blocks[0][2] = 1;			
		break;
		case 6:
			//Line
			block->blocks[0][1] = 1;
			block->blocks[1][1] = 1;
			block->blocks[2][1] = 1;	
			block->blocks[3][1] = 1;		
		break;
	}

	block->position.x = 3;
	block->position.y = 0;
	
	init_pair(1, COLOR_MAGENTA, COLOR_WHITE);
	init_pair(2, COLOR_YELLOW, COLOR_WHITE);
	init_pair(3, COLOR_BLUE, COLOR_WHITE);
	init_pair(4, COLOR_BLACK, COLOR_WHITE);
	init_pair(5, COLOR_RED, COLOR_WHITE);
	init_pair(6, COLOR_GREEN, COLOR_WHITE);
	init_pair(7, COLOR_CYAN, COLOR_WHITE);
	init_pair(8, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(9, COLOR_YELLOW, COLOR_BLACK);
	init_pair(10, COLOR_BLUE, COLOR_BLACK);
	init_pair(11, COLOR_WHITE, COLOR_BLACK);
	init_pair(12, COLOR_RED, COLOR_BLACK);
	init_pair(13, COLOR_GREEN, COLOR_BLACK);
	init_pair(14, COLOR_CYAN, COLOR_BLACK);
	init_pair(15, COLOR_WHITE, COLOR_WHITE);

	block->color = blockId +1;
}
//Set new values when restart game after fail.
void restart()
{
	point = 0;
	delay = 500;
	int x, y;
	for(y = 0; y < HEIGHT; y++)
	{
		for(x = 0; x < WIDTH; x++)
		{
			map[y][x] = 0;						
		}
	}
	newBlock(&block);
	newBlock(&nextblock);	
	for(y = 0; y < BLOCK_HEIGHT; y++)
	{
		for(x = 0; x < BLOCK_WIDTH; x++)
		{
			holdblock.blocks[y][x] = 0;
		}
	}
	holdblock.color = 0;
}

//check if game over and make game over screen
void gameOver()
{	
	int x;
	for(x = 0; x <= WIDTH; x++)
	{
		if(map[0][x] != 0)
		{	
			int i;
			for(i = 0; i < 7; i++)
			{
			attron(COLOR_PAIR(i+1+7));
			mvprintw(7+i,0,"GAMEOVER!GAMEOVER!");
			attroff(COLOR_PAIR(i+1+7));			
			}
			refresh();
			game = 0;
			while(game == 0)
			{
				mvprintw(16, 20, "PRESS R TO RESTART");	
				refresh();
				if(getch() == 'r')
				{	
					restart();
					game = 1;
				}	
			}
		}
	}	

}
//Merge block into game map
void mergeBlock() 
{
	int y, x;
	for(y = 0; y < BLOCK_HEIGHT; y++) {
		for(x = 0; x < BLOCK_WIDTH; x++) {
			int y1 = block.position.y + y;
			int x1 = block.position.x + x;

			if(block.blocks[y][x] == 1) {
				map[y1][x1] = block.color;
			}
			hold = 0;
		}
	}
}

//add points and increase speed
void points(rows)
{
	switch(rows)
	{
		case 1:
			point += 10;
			delay -= 10;
			break;
		case 2:
			point += 30;
			delay -= 20;
			break;
		case 3:
			point += 90;
			delay -= 40;
			break;
		case 4:
			point += 270;
			delay -= 70;
			break;
	}
	if(delay < 50)
	{
		delay = 50;
	}
	mvprintw(20, 20, "your score is: %d", point);
}

//fill filled lines in map.
void moverows(int start)
{
	int x,y;
	for(y = start; y > 0; y--)
	{
		for(x = 0; x < WIDTH; x++)
		{
			map[y][x] = map[y-1][x];	
		}
	}

}
//Check if line is filled
void checkIfFullLine()
{	
	int x, y, isFilled, rows = 0;
	for(y = 0; y < HEIGHT; y++)
	{	
		isFilled = 0;
		for(x = 0; x < WIDTH; x++)
		{
			if(map[y][x] != 0)
			{
				isFilled++;		
			}
			else
			{
				break;			
			}
		}
		if(isFilled == WIDTH)
		{
			rows++;
			moverows(y);
		}
	}
	points(rows);
}
//Check for block crash
int hasCrashed() 
{
	int y, x;
	for(y = 0; y < BLOCK_HEIGHT; y++) {
		for(x = 0; x < BLOCK_WIDTH; x++) {
			int y1 = block.position.y + y;
			int x1 = block.position.x + x;

			if(block.blocks[y][x] != 0 && 
				(map[y1][x1] != 0 || y1 >= HEIGHT)) {
				return 1;
			}
		}
	}

	return 0;
}
//Check if brick is inside map
int isWithinMap() {
	int x, y;
	for(y = 0; y < BLOCK_HEIGHT; y++) {
		for(x = 0; x < BLOCK_WIDTH; x++) {
			int y1 = block.position.y + y;
			int x1 = block.position.x + x;

			if(block.blocks[y][x] == 1 && (x1 > (WIDTH - 1) || x1 < 0)) {
				return 0;
			}
		}
	}
	return 1;
}
//Rotate a block
void rotateBlock() 
{
	Block tempBlock = block;
	int i, j;
	for (i = 0; i < BLOCK_HEIGHT; i++)
	{
		for(j = 0; j < BLOCK_WIDTH; j++) 
		{
			block.blocks[i][j] = tempBlock.blocks[3 - j][i];
		}
	}
	
	if(!isWithinMap()) 
	{
		block = tempBlock;
	}
}

void renderChar(char ch, int x, int y) { mvaddch(y, x, ch); }

//Render the view of the next available block
void renderNextBlock()
{
		mvprintw(1, 20, "Next block is:");
		int x, y;
		for(y = 0; y < BLOCK_HEIGHT; y++) 
		{
			for(x = 0; x < BLOCK_HEIGHT; x++) 
			{
				int y1 = 2 + y;
				int x1 = 25 + x;			
			
				attron(COLOR_PAIR(nextblock.color + 7));
				if(nextblock.blocks[y][x] == 1) {
					renderChar('X', x1, y1);
				}
				attroff(COLOR_PAIR(nextblock.color + 7));
			}
		}
}
//Render the holded block, if any
void renderHoldBlock()
{
		mvprintw(6, 20, "Holded block is:");
		int x, y;
		for(y = 0; y < BLOCK_HEIGHT; y++) 
		{
			for(x = 0; x < BLOCK_HEIGHT; x++) 
			{
				int y1 = 8 + y;
				int x1 = 25 + x;			
			
				attron(COLOR_PAIR(holdblock.color + 7));
				if(holdblock.blocks[y][x] == 1) 
				{
					renderChar('X', x1, y1);
				}
				attroff(COLOR_PAIR(holdblock.color + 7));
			}
		}
}
//User input
int handleInput(int times) 
{
	int ch = getch();
	switch(ch) {
		case KEY_LEFT:
			block.position.x--;
			times ++;
			if(hasCrashed() || !isWithinMap()) block.position.x++;
		break;
		case KEY_RIGHT:
			block.position.x++;
			times++;
			if(hasCrashed() || !isWithinMap()) block.position.x--;
		break;
		case KEY_DOWN:
			block.position.y++;
			if(hasCrashed()) block.position.y--;
		break;
		case KEY_UP:
			times++;
			rotateBlock();
		break;
		case 'c':
			if(hold == 0)
			{
				if(holdblock.color == 0)
				{
					holdblock = block;
					block = nextblock;
					newBlock(&nextblock);
				}
				else
				{
					Block tempblock;				
					tempblock = block;
					block = holdblock;
					holdblock = tempblock;
				}
					block.position.x = 3;
					block.position.y = 0;
					hold = 1;
			}
		break;
		case 'p':
			mvprintw(5,0,"PRESS P TO RESUME");
			while(1)
			{
				if(getch() == 'p')
				{
					break;				
				}
			}
		break;
		case 'r':
			refresh();
		break;
	}
	return times;
}

//Update the posistion y
void update() 
{
	block.position.y++;
	if(hasCrashed()) 
{
		block.position.y--;
		mergeBlock();
		block = nextblock;
		newBlock(&nextblock);
	}
}
//Render map
void RenderMap() {
	// Render edges
	int y,x;
	for(y = 0; y < HEIGHT + 2; y++) 
	{
		if(y == 0 || y == HEIGHT + 1) 
		{
			for(x = 0; x <= WIDTH + 1; renderChar('-', x, y), x++);
		} else {
			renderChar('|',0, y);
			renderChar('|', WIDTH + 1, y);
		}
	}
	// Render values
	y = 0;
	x = 0;
	for(y = 0; y < HEIGHT; y++) 
	{
		for(x = 0; x < WIDTH; x++) 
		{
			if(map[y][x] == 0) 
			{
				attron(COLOR_PAIR(15));
				renderChar(' ', x + 1, y + 1);
				attron(COLOR_PAIR(15));
			} else {
				int color = map[y][x];
				attron(COLOR_PAIR(color));
				renderChar('X', x + 1, y + 1);
				attroff(COLOR_PAIR(color));
			}
		}
	}
}
//render block
void RenderBlock() 
{
	int x, y;
	for(y = 0; y < BLOCK_HEIGHT; y++) 
	{
		for(x = 0; x < BLOCK_HEIGHT; x++) 
		{
			int y1 = block.position.y + y + 1;
			int x1 = block.position.x + x + 1;			
			
			attron(COLOR_PAIR(block.color));
			if(block.blocks[y][x] == 1) 
			{
				renderChar('X', x1, y1);
			}
			attroff(COLOR_PAIR(block.color));
		}
	}
}
//main render method
int Render() 
{
	erase();
	checkIfFullLine();
	RenderMap();
	RenderBlock();
	renderNextBlock();
	renderHoldBlock();
	mvprintw(12, 20, "Move with arrow keys");
	mvprintw(13, 20, "Rotate with arrow up");
	mvprintw(14, 20, "Press C to hold brick");
	mvprintw(15, 20, "Press P to pause/resume game");
	gameOver();
	refresh();
} 
//Play music in background
void playmusic()
{
	if(isPlaying == 0)
	{
		inst = libvlc_new(0, NULL);
		media = libvlc_media_new_path(inst, "Tetris.mp3");
		mediaplayer = libvlc_media_player_new_from_media(media);
		libvlc_media_release(media);	    
		libvlc_media_player_play(mediaplayer);
		isPlaying = 1;
	}
	//times implemented because of buffertime on media player
	if(times > 20)
	{
		isPlaying = libvlc_media_player_is_playing(mediaplayer);
		times  = 0;
	}
	times++;
	
}

//game loop
int loop() 
{
	int times = 0;
	
	while(game) 
	{
		times = handleInput(times);
		update();
		timeout(delay);
			playmusic();
		if (times == 2)
		{
			block.position.y--;
			times = 0;
		}
		Render();
	}
}

//Main method, with star en stop librarys and call game loop
int main(int argc, char** argv) 
{

	//init befor loop
	initscr();
	start_color();
	noecho();
	cbreak();
	curs_set(FALSE);
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);	
	srand(time(NULL));
	newBlock(&block);
	newBlock(&nextblock);


	//Loop
	loop();

	//ending after loop is over
	endwin();
	libvlc_media_player_stop(mediaplayer);
    libvlc_media_player_release(mediaplayer);
    libvlc_release(inst);

	return 0;
}
