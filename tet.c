#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

#define delay 1000
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
void NewBlock(Block * block) {
	memset(block, 0, sizeof(Block));		

	int block_id = rand() % 7;

	switch(block_id) {
		default:
			block->blocks[0][1] = 1;
			block->blocks[1][1] = 1;
			block->blocks[1][2] = 1;
			block->blocks[2][1] = 1;			
		break;
	}

	block->position.x = 3;
	block->position.y = 0;

	//attron(1);
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

void HandleInput() {
	int ch = getch();
	switch(ch) {
		case KEY_LEFT:
			block.position.x--;
			if(HasCrashed() || !IsWithinMap()) block.position.x++;
		break;
		case KEY_RIGHT:
			block.position.x++;
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
				RenderChar('.', x + 1, y + 1);
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

			if(block.blocks[y][x] == 1) {
				RenderChar('@', x1, y1);
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

	NewBlock(&block);

	loop();

	endwin();

	return 0;
}
