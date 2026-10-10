#include <rinyTenderer/window.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>


typedef struct terminalContext
{
  struct termios  origTermios;
  TerminalSize    size;
  TerminalSize    cursorPos;

#ifdef DEBUG
  const char*     logFilePath;
  FILE*           logFile; 
#endif
} TerminalCTX;

static bool        enable  = false;
static TerminalCTX ctx;

#define ENSURE_AFTER_INIT JUST_ASSERT_DEBUG_MESSAGE(enable, "[TERMNIAL] : Call terminalInit() first")

bool terminalEnableRawMode(void)
{
  struct termios copy = ctx.origTermios;

  copy.c_lflag      &= ~(ECHO | ICANON);
  copy.c_cc[VMIN]    = 0;
  copy.c_cc[VTIME]   = 0;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &copy) == -1)
  {
    JUST_LOG_FATAL("[TERMINAL] : Failed to enable raw mode");
    return false;
  }

  return true;
}

bool terminalDisableRawMode(void)
{
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &ctx.origTermios) == -1)
  {
    JUST_LOG_FATAL("[TERMINAL] : Failed to disable raw mode");
    return false;
  }

  return true;
}

JUST_API void terminalInit(void)
{
  JUST_ASSERT_DEBUG_MESSAGE(enable == false, "[TERMINAL] : Init called twice");

#ifdef DEBUG 
  ctx.logFilePath = "log.txt";
  ctx.logFile = fopen(ctx.logFilePath, "w");
  JUST_ASSERT_DEBUG_MESSAGE(ctx.logFile != NULL, "[TERMINAL] : Failed to open a log file");
  justLogSetOutputStream(ctx.logFile);
#endif

  JUST_LOG_DEBUG("[TERMINAL] : Intializing");

  JUST_ASSERT_MESSAGE((tcgetattr(STDIN_FILENO, &ctx.origTermios) != -1), "[TERMINAL] : Failed to save original state");
  JUST_ASSERT_DEBUG(terminalEnableRawMode());
  terminalHideCursor();
  terminalClear();

  enable = true;

  JUST_LOG_INFO("[TERMINAL] : Initialized");
}

JUST_API void terminalShutdown(void)
{
  ENSURE_AFTER_INIT;
  JUST_LOG_DEBUG("[TEMRINAL] : Shutting down");

  JUST_ASSERT_DEBUG(terminalDisableRawMode());
  terminalShowCursor();
  terminalClear();
  enable = false;

  JUST_LOG_INFO("[TERMINAL] : Shutdown");

#ifdef DEBUG
  justLogSetOutputStream(stdout);
  fclose(ctx.logFile);
#endif
}

JUST_API void terminalUpdateSize(void)
{
  struct winsize windowSize;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize);

  if (windowSize.ws_row != ctx.size.rows || windowSize.ws_col != ctx.size.cols)
  {
    JUST_LOG_INFO("[TERMINAL] : Size updated to %u rows and %u cols", windowSize.ws_row, windowSize.ws_col);
  }
  ctx.size.rows = windowSize.ws_row;
  ctx.size.cols = windowSize.ws_col;
}

JUST_API void terminalSetCursor(TerminalSize POSITION)
{
  ENSURE_AFTER_INIT;

  ctx.cursorPos = POSITION;
  printf("\033[%d;%dH", POSITION.rows, POSITION.cols);
}

