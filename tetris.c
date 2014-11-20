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

//find other place for this
int hold = 0;
int stoploop = 1;
int pauseKey = 0;
int point;
int isPlaying = 0;
int times;
int game = 1;


void NewBlock(Block * block) {
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
void restart()
{
	point = 0;
	int x, y;
	for(y = 0; y < HEIGHT; y++)
	{
		for(x = 0; x < WIDTH; x++)
		{
			map[y][x] = 0;						
		}
	}
	NewBlock(&block);
	NewBlock(&nextblock);
	Block temp;
	holdblock = temp;
}
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
void MergeBlock() 
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
void points(rows)
{
	switch(rows)
	{
		case 1:
			point += 10;
			delay -= 20;
			break;
		case 2:
			point += 30;
			delay -= 40;
			break;
		case 3:
			point += 90;
			delay -= 60;
			break;
	}
	if(delay < 50)
	{
		delay = 50;
	}
	mvprintw(20, 20, "your score is: %d", point);
}
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
int HasCrashed() 
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

int IsWithinMap() {
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

void RotateBlock() 
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
	
	if(!IsWithinMap()) 
	{
		block = tempBlock;
	}
}

void RenderChar(char ch, int x, int y) { mvaddch(y, x, ch); }

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
					RenderChar('X', x1, y1);
				}
				attroff(COLOR_PAIR(nextblock.color + 7));
			}
		}
}

void renderHoldBlock()
{
		mvprintw(6, 20, "Holded block is:");
		int x, y;
		for(y = 0; y < BLOCK_HEIGHT; y++) 
		{
			for(x = 0; x < BLOCK_HEIGHT; x++) 
			{
				int y1 = 7 + y;
				int x1 = 25 + x;			
			
				attron(COLOR_PAIR(holdblock.color + 7));
				if(holdblock.blocks[y][x] == 1) 
				{
					RenderChar('X', x1, y1);
				}
				attroff(COLOR_PAIR(holdblock.color + 7));
			}
		}
}

int HandleInput(int times) 
{
	int ch = getch();
	switch(ch) {
		case KEY_LEFT:
			block.position.x--;
			times ++;
			if(HasCrashed() || !IsWithinMap()) block.position.x++;
		break;
		case KEY_RIGHT:
			block.position.x++;
			times++;
			if(HasCrashed() || !IsWithinMap()) block.position.x--;
		break;
		case KEY_DOWN:
			block.position.y++;
			if(HasCrashed()) block.position.y--;
		break;
		case KEY_UP:
			times++;
			RotateBlock();
		break;
		case 'c':
			if(hold == 0)
			{
				if(holdblock.color == 0)
				{
					holdblock = block;
					block = nextblock;
					NewBlock(&nextblock);
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

void Update() 
{
	block.position.y++;
	if(HasCrashed()) 
{
		block.position.y--;
		MergeBlock();
		block = nextblock;
		NewBlock(&nextblock);
	}
}
void RenderMap() {
	// Render edges
	int y,x;
	for(y = 0; y < HEIGHT + 2; y++) 
	{
		if(y == 0 || y == HEIGHT + 1) 
		{
			for(x = 0; x <= WIDTH + 1; RenderChar('-', x, y), x++);
		} else {
			RenderChar('|',0, y);
			RenderChar('|', WIDTH + 1, y);
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
				RenderChar(' ', x + 1, y + 1);
				attron(COLOR_PAIR(15));
			} else {
				int color = map[y][x];
				attron(COLOR_PAIR(color));
				RenderChar('X', x + 1, y + 1);
				attroff(COLOR_PAIR(color));
			}
		}
	}
}

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
				RenderChar('X', x1, y1);
			}
			attroff(COLOR_PAIR(block.color));
		}
	}
}
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
	if(times > 20)
	{
		isPlaying = libvlc_media_player_is_playing(mediaplayer);
		times  = 0;
	}
	times++;
	
}
int loop() 
{
	int times = 0;
	
	while(game) 
	{
		times = HandleInput(times);
		Update();
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
int main(int argc, char** argv) 
{
	initscr();
	
	start_color();
	noecho();
	cbreak();
	curs_set(FALSE);
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);	
	srand(time(NULL));

	NewBlock(&block);
	NewBlock(&nextblock);



	loop();

	endwin();
	libvlc_media_player_stop(mediaplayer);
    libvlc_media_player_release(mediaplayer);
    libvlc_release(inst);

	return 0;
}
