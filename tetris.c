#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define delay 1000
#define WIDTH 10
#define HEIGHT 20
#define BRICK_WIDTH 3
#define BRICK_HEIGHT 3

typedef unsigned int Byte;
typedef unsigned char Color;

typedef struct {
	int x;
	int y;
} Position;

typedef struct {
	Byte bricks[BRICK_WIDTH][BRICK_HEIGHT];
	Color color;
	Position position;
} Block;

typedef Byte Map[HEIGHT][WIDTH];

Block block;
Map map;


void HandleInput() {
	int ch = getch();
	switch(ch) {
		case KEY_LEFT:
			block.position.x--;
		break;
		case KEY_RIGHT:
			block.position.x++;
		break;
		case KEY_DOWN:
			block.position.y++;
		break;
	}
}

void Update() {
	block.position.y++;
	if(block.position.y > (HEIGHT - 2)) {
		block.position.y = 1;
	}		
}

void RenderChar(char ch, int x, int y) {
	mvaddch(y, x, ch);
}

void RenderMap() {
	for(int y = 0; y < HEIGHT; y++) {
		if(y == 0 || y == (HEIGHT - 1)) {
			for(int x = 0; x < WIDTH; x++,RenderChar('-', x, y));
		} else {
			RenderChar('|', 0, y);
			RenderChar('|', WIDTH, y);
		}
	}
}

void RenderBlock() {
	for(int y = 0; y < BRICK_HEIGHT; y++) {
		for(int x = 0; x < BRICK_HEIGHT; x++) {
			int y1 = block.position.y + y;
			int x1 = block.position.x + x;			

			if(block.bricks[y][x] == 1) {
				RenderChar('@', 
					block.position.x + x, 
					block.position.y + y);
			}
		}
	}
}

void Render() {
	erase();
	RenderMap();
	RenderBlock();
	refresh();
} 


int loop() {
	while(1) {
		HandleInput();
		Update();
		Render();
	}
}

void NewBrick(Block * block) {
	memset(block->bricks, 0, BRICK_WIDTH * BRICK_HEIGHT);		

	int block_id = rand() % 7;

	switch(block_id) {
		default:
			block->bricks[0][1] = 1;
			block->bricks[1][1] = 1;
			block->bricks[2][1] = 1;			
		break;
	}

	//attron(block_id);
}


int main(int argc, char** argv) {
	initscr();

	init_pair(0, COLOR_BLACK, COLOR_RED);
	init_pair(1, COLOR_BLACK, COLOR_GREEN);

	noecho();
	cbreak();
	curs_set(FALSE);
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	timeout(delay);	

	NewBrick(&block);
	block.position.x = 5;
	block.position.y = 2;

	loop();

	endwin();

	return 0;
}
