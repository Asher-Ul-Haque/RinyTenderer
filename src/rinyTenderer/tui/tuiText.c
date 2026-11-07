#include "justLibrary.h"
#include "rinyTenderer/_structs.h"
#include <rinyTenderer/tui/tui.h>
#include <rinyTenderer/renderer/frame.h>
#include <stddef.h>
#include <string.h>

JUST_API void tuiDrawText(
    TerminalFrame*  FRAME,
    FramePos        POSITION,
    const char*     TEXT,
    size_t          TEXT_LENGTH,
    TerminalColor   FG_COLOR,
    TerminalColor   BG_COLOR)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG(TEXT != NULL);

  float curX = POSITION.x;
  float curY = POSITION.y;
  float curZ = POSITION.z;

  size_t length = strnlen(TEXT, TEXT_LENGTH);
  for (size_t i = 0; i < length; ++i)
  {
    if (TEXT[i] == '\n')
    {
      curX = POSITION.x;
      curY += 2.0f;
      continue;
    }

    char cStr[2] = { TEXT[i], '\0' };
    frameSetChar(
        FRAME, 
        (FramePos) 
        {
          .x = curX,
          .y = curY,
          .z = curZ
        }, 
        cStr, 
        FG_COLOR, 
        BG_COLOR);
    curX += 1.0f;
  }
}

JUST_API void tuiDrawTextCentered(
    TerminalFrame*  FRAME,
    FramePos        POSITION,
    uint16_t        WIDTH,
    const char*     TEXT,
    size_t          TEXT_LENGTH,
    TerminalColor   FG_COLOR,
    TerminalColor   BG_COLOR)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG(TEXT != NULL);

  size_t length = strnlen(TEXT, TEXT_LENGTH);
  size_t index  = 0;
  while (index < length && TEXT[index] != '\n') index++;

  float offsetX = 0.0f;
  if (WIDTH > index)
  {
    offsetX = (float)((WIDTH - index) / 2.0f);
  }

  FramePos centeredPos = 
  {
    .x = POSITION.x + offsetX,
    .y = POSITION.y,
    .z = POSITION.z
  };
  tuiDrawText(FRAME, centeredPos, TEXT, index, FG_COLOR, BG_COLOR);
}

