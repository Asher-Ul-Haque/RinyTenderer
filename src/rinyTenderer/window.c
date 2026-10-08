#include <rinyTenderer/window.h>
#include <sys/ioctl.h>
#include <unistd.h>

static TerminalSize _size;

JUST_API void terminalUpdateSize(void)
{
  struct winsize windowSize;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize);

  if (windowSize.ws_row != _size.rows || windowSize.ws_col != _size.cols)
  {
    JUST_LOG_INFO("[TERMINAL] : Size updated to %u rows and %u cols", windowSize.ws_row, windowSize.ws_col);
  }
  _size.rows = windowSize.ws_row;
  _size.cols = windowSize.ws_col;
}
