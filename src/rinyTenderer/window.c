#include "justLibrary.h"
#include "rinyTenderer/_structs.h"
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

  // - - - Enable mouse tracking: 1000 (click/release), 1002 (drag), 1006 (SGR extended format)
  fputs("\033[?1000h\033[?1002h\033[?1006h", stdout);
  fflush(stdout);

  return true;
}

static bool terminalDisableRawMode(void)
{
  // - - - Disable mouse tracking before resetting
  fputs("\033[?1000l\033[?1002l\033[?1006l", stdout);
  fflush(stdout);

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &ctx.origTermios) == -1)
  {
    JUST_LOG_FATAL("[TERMINAL] : Failed to disable raw mode");
    return false;
  }

  return true;
}

static void terminalInputInit(void)
{
  bool ok = justBitsetCreate(&ctx.keys, KEY_CODE_COUNT, NULL, RINY_TENDERER_TAG);
  JUST_ASSERT_DEBUG(ok);

  ok = justBitsetCreate(&ctx.mouseButtons, MOUSE_BUTTON_COUNT, NULL, RINY_TENDERER_TAG);
  JUST_ASSERT_DEBUG(ok);

  justBitsetClearAll(&ctx.keys);
  justBitsetClearAll(&ctx.mouseButtons);
  ctx.textLength  = 0;
  ctx.scrollDelta = 0;
  memset(ctx.textBuffer, '\0', sizeof(ctx.textBuffer));
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
  justMemorySetLimit(mb_1, RINY_TENDERER_TAG);
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

  if (windowSize.ws_row != ctx.size.y || windowSize.ws_col != ctx.size.x)
  {
    JUST_LOG_INFO("[TERMINAL] : Size updated to %u rows and %u cols", windowSize.ws_row, windowSize.ws_col);
  }
  ctx.size.y = windowSize.ws_row;
  ctx.size.x = windowSize.ws_col;
}

JUST_API void terminalSetCursor(TerminalSize POSITION)
{  
  ENSURE_AFTER_INIT;

  ctx.cursorPos = POSITION;
  printf("\033[%d;%dH", (int)POSITION.y, (int)POSITION.x);
}

JUST_API void terminalPollInput(void)
{
  ENSURE_AFTER_INIT;

  justBitsetClearAll(&ctx.keys);
  justBitsetClearAll(&ctx.mouseButtons);
  ctx.textLength = 0;
  ctx.scrollDelta = 0;
  memset(ctx.textBuffer, 0, sizeof(ctx.textBuffer));

  // - - - Drain all bytes in stdin for this frame
  char c;
  while (read(STDIN_FILENO, &c, 1) > 0)
  {
    // - - - Handle escape / arrow sequences for keys bitset
    if (c == '\033')
    {
      char next;
      if (read(STDIN_FILENO, &next, 1) != 1) continue;

      if (next == '[')
      {
        char modeChar;
        if (read(STDIN_FILENO, &modeChar, 1) == 1 && modeChar == '<')
        {
          // - - - Read SGR parameters: Cb;Cx;Cy[M|m]
          int cb = 0, cx = 0, cy = 0;
          char termType = '\0';

          // - - - Scan for Cb;Cx;CyM
          char buf[32];
          int  idx = 0;
          while (idx < (int)sizeof(buf) - 1)
          {
            char subC;
            if (read(STDIN_FILENO, &subC, 1) != 1) break;
            buf[idx++] = subC;
            if (subC == 'M' || subC == 'm')
            {
              termType = subC;
              break;
            }
          }
          buf[idx] = '\0';

          if (termType != '\0' && sscanf(buf, "%d;%d;%d", &cb, &cx, &cy) == 3)
          {
            ctx.mousePos = (TerminalPos){.x = cx, .y = cy};

            bool        isPress = (termType == 'M');
            MouseButton btn     = MOUSE_BUTTON_COUNT;

            // - - - Decode Cb button flags
            if ((cb & 0x40) != 0)
            {
              // - - - scroll wheel events
              if ((cb & 1) == 0) 
              { 
                ctx.scrollDelta = 1; 
                justBitsetSet(&ctx.mouseButtons, MOUSE_BUTTON_SCROLL_UP); 
              }
              else
              { 
                ctx.scrollDelta = -1; 
                justBitsetSet(&ctx.mouseButtons, MOUSE_BUTTON_SCROLL_DOWN);
              }
            }
            else
            {
              int buttonCode = cb & 3;
              if      (buttonCode == 0) btn = MOUSE_BUTTON_LEFT;
              else if (buttonCode == 1) btn = MOUSE_BUTTON_MIDDLE;
              else if (buttonCode == 2) btn = MOUSE_BUTTON_RIGHT;

              if (btn < MOUSE_BUTTON_COUNT && isPress)
              {
                justBitsetSet(&ctx.mouseButtons, btn);
              }
            }
          }
          continue;
        }

        // - - - Handle arrow keys / navigation escape sequences
        char seq[2];
        seq[0] = modeChar;

        if (read(STDIN_FILENO, &seq[1], 1) == 1)
        {
          KeyCode code = KEY_UNKNOWN;
          if (seq[0] == '[')
          {
            switch (seq[1])
            {
              case 'A': code = KEY_UP;    break;
              case 'B': code = KEY_DOWN;  break;
              case 'C': code = KEY_RIGHT; break;
              case 'D': code = KEY_LEFT;  break;
              default :                   break;
            }
          }
          if (code != KEY_UNKNOWN) justBitsetSet(&ctx.keys, code);
        }
      }
      else justBitsetSet(&ctx.keys, KEY_ESCAPE);
      continue;
    }

    // - - - Map regular keys to bitset
    KeyCode code = KEY_UNKNOWN;
    if      (c == '\n'  || c == '\r') { code = KEY_ENTER; }
    else if (c == 127   || c == '\b') { code = KEY_BACKSPACE; }
    else if (c == ' ')                { code = KEY_SPACE; }
    else if (c >= 'a' && c <= 'z')    { code = KEY_A + (c - 'a'); }
    else if (c >= 'A' && c <= 'Z')    { code = KEY_A + (c - 'A'); }
    else if (c >= '0' && c <= '9')    { code = KEY_A + (c - '0'); }
    else if (c == 'q' || c == 'Q')    { code = KEY_Q; }

    if (code > KEY_NONE && code < KEY_CODE_COUNT)
    {
      justBitsetSet(&ctx.keys, code);
    }

    // - - - 3. Accumulate printable chars & backspace / enter into the text buffer
    int asciiPrintableStart = 32;
    int asciiPrintableEnd   = 126;
    if (
        (
          (c >= asciiPrintableStart && c <= asciiPrintableEnd) || 
          c == '\n'                                            || 
          c == '\r'                                            || 
          c == 127                                             || 
          c == '\b') && 
        ctx.textLength < sizeof(ctx.textBuffer) - 1)
    {
      ctx.textBuffer[ctx.textLength++]  = c;
    }
  }
}

JUST_API bool terminalIsKeyDown(KeyCode KEY)
{
  ENSURE_AFTER_INIT;

  JUST_ASSERT_DEBUG(KEY > KEY_NONE);
  JUST_ASSERT_DEBUG(KEY < KEY_CODE_COUNT);

  return justBitsetGet(&ctx.keys, KEY);
}

JUST_API const char* terminalGetText(void)
{
  ENSURE_AFTER_INIT;

  return ctx.textBuffer;
}

JUST_API size_t terminalGetTextLength(void)
{
  ENSURE_AFTER_INIT;

  return ctx.textLength;
}

JUST_API TerminalPos terminalGetMousePos(void)
{
  ENSURE_AFTER_INIT;

  return ctx.mousePos;
}

JUST_API bool terminalIsMouseButtonDown(MouseButton BUTTON)
{
  ENSURE_AFTER_INIT;

  JUST_ASSERT_DEBUG(BUTTON >= 0);
  JUST_ASSERT_DEBUG(BUTTON < MOUSE_BUTTON_COUNT);

  return justBitsetGet(&ctx.mouseButtons, BUTTON);
}

JUST_API int terminalGetScrollDelta(void)
{
  ENSURE_AFTER_INIT;

  return ctx.scrollDelta;
}
