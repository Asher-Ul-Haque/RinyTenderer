#include <math/vector.hpp>
#include <defines.hpp>
#include <curses.h>
#include <utils/common.hpp>

i32 main (void) 
{
  LOG_DEBUG("Trying to get the first ncurses window up");

  initscr();
  raw();
  noecho();

  printw("This is a test of ncurses");
  getch();

  endwin();

  Vector<f32, 3> a{1, 2, 2};
  Vector<f32, 3> b(0);

  f32 ans = a.angleDegrees(b);

  LOG_INFO("Ran! " << ans);
  return 0;
}
