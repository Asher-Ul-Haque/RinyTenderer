#include <defines.hpp>
#include <utils/logger.hpp>
#include <utils/dataTypes.hpp>
#include <curses.h>

i32 main (void) 
{
  FORGE_LOG_DEBUG("Trying to get the first ncurses window up");

  initscr();
  raw();
  noecho();

  printw("This is a test of ncurses");
  getch();

  endwin();

  FORGE_LOG_INFO("Ran!");
  return 0;
}
