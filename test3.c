#include <ncurses.h>

int main()
{
initscr();
  noecho();
  curs_set(FALSE);
  sleep(1);
  endwin();
}
