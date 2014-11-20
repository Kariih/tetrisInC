#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 10
#define HEIGHT 20
#define BLOCK_WIDTH 3
#define BLOCK_HEIGHT 3

typedef unsigned int Byte;
typedef unsigned char Color;

typedef struct {
	int x;
	int y;
} Position;

typedef struct {
	Byte blocks[BLOCK_WIDTH][BLOCK_HEIGHT];
	Color color;
	Position position;
} Block;

typedef Byte Map[HEIGHT][WIDTH];

Block block;
Map map;
int delay = 500;

//find other place for this
int point;

void NewBlock(Block * block) {
	memset(block, 0, sizeof(Block));		

	int blockId = rand() % 7;

	switch(blockId) {
		case 0:
			//T block
			block->blocks[2][0] = 1;
			block->blocks[2][1] = 1;
			block->blocks[2][2] = 1;
			block->blocks[1][1] = 1;			
		break;
		case 1:
			//Cube
			block->blocks[1][0] = 1;
			block->blocks[1][1] = 1;
			block->blocks[2][0] = 1;
			block->blocks[2][1] = 1;			
		break;
		case 2:
			//J block
			block->blocks[1][0] = 1;
			block->blocks[1][1] = 1;
			block->blocks[1][2] = 1;
			block->blocks[2][2] = 1;			
		break;
		case 3:
			//L block
			block->blocks[1][2] = 1;
			block->blocks[2][0] = 1;
			block->blocks[2][1] = 1;
			block->blocks[2][2] = 1;			
		break;
		case 4:
			//Z block
			block->blocks[1][0] = 1;
			block->blocks[1][1] = 1;
			block->blocks[2][1] = 1;
			block->blocks[2][2] = 1;			
		break;
		case 5:
			//S block
			block->blocks[2][0] = 1;
			block->blocks[2][1] = 1;
			block->blocks[1][1] = 1;
			block->blocks[1][2] = 1;			
		break;
		case 6:
			//Line
			block->blocks[0][1] = 1;
			block->blocks[1][1] = 1;
			block->blocks[2][1] = 1;			
		break;
	}

	block->position.x = 3;
	block->position.y = 0;

	init_pair(1, COLOR_CYAN, COLOR_CYAN);
	init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(3, COLOR_GREEN, COLOR_GREEN);
	init_pair(4, COLOR_RED, COLOR_RED);
	init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(6, COLOR_BLUE, COLOR_BLUE);
	init_pair(7, COLOR_WHITE, COLOR_WHITE);

	block->color = blockId +1;
}
void gameOver()
{	
	int x;
	for(x = 0; x <= WIDTH; x++)
	{
		if(map[0][x] == 1)
		{	
			int i;
			for(i = 0; i < 5; i++)
			{
			mvprintw(10+i,0,"GAME OVER! GAME OVER! GAME OVER!");
			refresh();				
			}
		}
	}


}
void MergeBlock() {
	int y, x;
	for(y = 0; y < BLOCK_HEIGHT; y++) {
		for(x = 0; x < BLOCK_WIDTH; x++) {
			int y1 = block.position.y + y;
			int x1 = block.position.x + x;

			if(block.blocks[y][x] == 1) {
				map[y1][x1] = 1;
			}
		}
	}
}
void points(rows)
{
	switch(rows)
	{
		case 1:
			point += 10;
			delay -= 15;
			break;
		case 2:
			point += 30;
			delay -= 30;
			break;
		case 3:
			point += 90;
			delay -= 45;
			break;
	}
	if(delay < 200)
	{
		delay = 200;
	}
	mvprintw(5, 15, "your score is: %d and speed is: %d", point, delay);
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
	refresh();
	int x, y, isFilled, rows = 0;
	for(y = 0; y < HEIGHT; y++)
	{	
		isFilled = 0;
		for(x = 0; x < WIDTH; x++)
		{
			if(map[y][x] == 1)
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
int HasCrashed() {
	int y, x;
	for(y = 0; y < BLOCK_HEIGHT; y++) {
		for(x = 0; x < BLOCK_WIDTH; x++) {
			int y1 = block.position.y + y;
			int x1 = block.position.x + x;

			if(block.blocks[y][x] == 1 && 
				(map[y1][x1] == 1 || y1 >= HEIGHT)) {
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

void RotateBlock() {
	Block tempBlock = block;
	int i, j;
	for (i = 0; i < BLOCK_HEIGHT; i++) {
		for(j = 0; j < BLOCK_WIDTH; j++) {
			block.blocks[i][j] = tempBlock.blocks[2 - j][i];
		}
	}
	
	if(HasCrashed() || !IsWithinMap()) {
		block = tempBlock;
	}
}

int HandleInput(int times) {
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
			RotateBlock();
		break;
	}
	return times;
}
void Update() {
	block.position.y++;
	if(HasCrashed()) {
		block.position.y--;
		MergeBlock();
		NewBlock(&block);
	}
}

void RenderChar(char ch, int x, int y) { mvaddch(y, x, ch); }

void RenderMap() {
	// Render edges
	int y,x;
	for(y = 0; y < HEIGHT + 2; y++) {
		if(y == 0 || y == HEIGHT + 1) {
			for(x = 0; x <= WIDTH + 1; RenderChar('#', x, y), x++);
		} else {
			RenderChar('|',         0, y);
			RenderChar('|', WIDTH + 1, y);
		}
	}
	// Render values
	y = 0;
	x = 0;
	for(y = 0; y < HEIGHT; y++) {
		for(x = 0; x < WIDTH; x++) {
			if(map[y][x] == 0) {
				RenderChar(' ', x + 1, y + 1);
			} else {
				RenderChar('#', x + 1, y + 1);
			}
		}
	}
}

void RenderBlock() {
	int x, y;
	for(y = 0; y < BLOCK_HEIGHT; y++) {
		for(x = 0; x < BLOCK_HEIGHT; x++) {
			int y1 = block.position.y + y + 1;
			int x1 = block.position.x + x + 1;			
			
			attron(COLOR_PAIR(block.color));
			if(block.blocks[y][x] == 1) {
				RenderChar('#', x1, y1);
			}
			attroff(COLOR_PAIR(block.color));
		}
	}
}


void Render() {
	erase();
	checkIfFullLine();
	RenderMap();
	gameOver();
	RenderBlock();
	refresh();
} 
int loop() {
	int times = 0;
	while(1) {
		times = HandleInput(times);
		Update();
		timeout(delay);
		if (times == 2)
		{
			block.position.y--;
			times = 0;
		}
		Render();
	}
}



int main(int argc, char** argv) {
	initscr();

	start_color();
	noecho();
	cbreak();
	curs_set(FALSE);
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);	

	NewBlock(&block);

	loop();

	endwin();

	return 0;
}
