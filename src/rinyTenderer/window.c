#include "justLibrary.h"
#include <rinyTenderer/window.h>
#include <unistd.h>

static bool        enable  = false;
static TerminalCTX ctx;

JUST_LOCAL const TerminalCTX* terminalGetContext(void)
{ return &ctx; }

#define ENSURE_AFTER_INIT JUST_ASSERT_DEBUG_MESSAGE(enable, "[TERMNIAL] : Call terminalInit() first")

static bool terminalEnableRawMode(void)
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

static bool terminalDisableRawMode(void)
{
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &ctx.origTermios) == -1)
  {
    JUST_LOG_FATAL("[TERMINAL] : Failed to disable raw mode");
    return false;
  }

  return true;
}

static void terminalInputInit(void)
{
  bool ok = justBitsetCreate(&ctx.keys, KEY_CODE_COUNT, NULL, "TERMINAL_INPUT");
  JUST_ASSERT_DEBUG_MESSAGE(ok, "[TERMINAL] : Failed to create bitset for input");

  justBitsetClearAll(&ctx.keys);
}

static KeyCode parseKeyCode(void)
{
  char c;
  if (read(STDIN_FILENO, &c, 1) <= 0) return KEY_NONE;

  if (c == '\033')
  {
    char seq[2];
    if (read(STDIN_FILENO, &seq[0], 1) != 1) return KEY_UNKNOWN;
    if (read(STDIN_FILENO, &seq[1], 1) != 1) return KEY_UNKNOWN;

    if (seq[0] == '[')
    {
      switch (seq[1])
      {
        case 'A' : return KEY_UP;
        case 'B' : return KEY_DOWN;
        case 'C' : return KEY_RIGHT;
        case 'D' : return KEY_LEFT;
        default  : return KEY_UNKNOWN; 
      }
    }

    return KEY_UNKNOWN;
  }

  switch (c)
  {
    case 'q' : return KEY_QUIT;
    case ' ' : return KEY_SPACE;
    case 'w' : 
    case 'W' : return KEY_W;
    case 'a' :
    case 'A' : return KEY_A;
    case 's' :
    case 'S' : return KEY_S;
    case 'd' :
    case 'D' : return KEY_D;
    default  : return KEY_UNKNOWN;
  }
}


JUST_API void terminalInit(void)
{
  JUST_ASSERT_DEBUG_MESSAGE(enable == false, "[TERMINAL] : Init called twice");

#ifdef DEBUG 
  ctx.logFilePath = "log.txt";
  ctx.logFile     = fopen(ctx.logFilePath, "w");
  JUST_ASSERT_DEBUG_MESSAGE(ctx.logFile != NULL, "[TERMINAL] : Failed to open a log file");
  justLogSetOutputStream(ctx.logFile);
#endif

  JUST_LOG_DEBUG("[TERMINAL] : Intializing");

  terminalInputInit();
  terminalUpdateSize();

  size_t mb_1 = 1024 * 1024;
  justMemorySetLimit(mb_1, "TERM");
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

JUST_API void terminalPollInput(void)
{
  justBitsetClearAll(&ctx.keys);

  // - - - Drain all bytes in stdin for this frame
  KeyCode key = KEY_UNKNOWN;
  while (key != KEY_NONE)
  {
    key = parseKeyCode();

    if (key > KEY_NONE && key < KEY_CODE_COUNT)
    {
      justBitsetSet(&ctx.keys, key);
    }
  }
}

JUST_API bool terminalIsKeyDown(KeyCode KEY)
{
  if (KEY <= KEY_NONE || KEY >= KEY_CODE_COUNT) return false;
  return justBitsetGet(&ctx.keys, KEY);
}
