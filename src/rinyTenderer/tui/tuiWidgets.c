#include "justLibrary.h"
#include "rinyTenderer/_structs.h"
#include <rinyTenderer/renderer/frame.h>
#include <rinyTenderer/tui/tui.h>
#include <stdint.h>

JUST_API void tuiDrawProgressBar(
    TerminalFrame*  FRAME,
    FramePos        POSITION,
    uint16_t        WIDTH,
    float           PROGRESS,
    TerminalColor   FILL_COLOR,
    TerminalColor   EMPTY_COLOR)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);

  if (PROGRESS > 1.0f) PROGRESS = 1.0f;
  if (PROGRESS < 0.0f) PROGRESS = 0.0f;

  uint16_t filledChars = (uint16_t)((float) WIDTH * PROGRESS);
  for (uint16_t i = 0; i < WIDTH; ++i)
  {
    FramePos cellPos = 
      {
        .x = POSITION.x + i,
        .y = POSITION.y,
        .z = POSITION.z
      };
    if (i < filledChars)
    {
      frameSetChar(FRAME, cellPos, "▓", FILL_COLOR, TERMINAL_COLOR_TRANSPARENT);
    }
    else
    {
      frameSetChar(FRAME, cellPos, "░", FILL_COLOR, TERMINAL_COLOR_TRANSPARENT);
    }
  }
}
