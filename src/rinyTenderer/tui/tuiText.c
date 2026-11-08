#include "justLibrary.h"
#include "rinyTenderer/_structs.h"
#include <rinyTenderer/tui/tui.h>
#include <rinyTenderer/renderer/frame.h>
#include <stddef.h>
#include <string.h>

JUST_API void tuiDrawText(TerminalFrame* FRAME, TuiText TEXT_DESC)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG(TEXT_DESC.text != NULL);

  float curX = TEXT_DESC.position.x;
  float curY = TEXT_DESC.position.y;
  float curZ = TEXT_DESC.position.z;

  size_t length = strnlen(TEXT_DESC.text, TEXT_DESC.maxLen);
  for (size_t i = 0; i < length; ++i)
  {
    if (TEXT_DESC.text[i] == '\n')
    {
      curX = TEXT_DESC.position.x;
      curY += 2.0f;
      continue;
    }

    char cStr[2] = { TEXT_DESC.text[i], '\0' };
    frameSetChar(
        FRAME, 
        (FramePos) 
        {
          .x = curX,
          .y = curY,
          .z = curZ
        }, 
        cStr, 
        TEXT_DESC.fgColor, 
        TEXT_DESC.bgColor);
    curX += 1.0f;
  }
}

JUST_API void tuiDrawTextCentered(TerminalFrame* FRAME, TuiTextCentered TEXT_DESC)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG(TEXT_DESC.text != NULL);

  size_t length = strnlen(TEXT_DESC.text, TEXT_DESC.maxLen);
  size_t index  = 0;
  while (index < length && TEXT_DESC.text[index] != '\n') index++;

  float offsetX = 0.0f;
  if (TEXT_DESC.width > index)
  {
    offsetX = (float)((TEXT_DESC.width - index) / 2.0f);
  }

  FramePos centeredPos = 
  {
    .x = TEXT_DESC.position.x + offsetX,
    .y = TEXT_DESC.position.y,
    .z = TEXT_DESC.position.z
  };
  tuiDrawText(
      FRAME, 
      (TuiText)
      {
        .text     = TEXT_DESC.text,
        .maxLen   = TEXT_DESC.maxLen,
        .position = centeredPos,
        .bgColor  = TEXT_DESC.bgColor,
        .fgColor  = TEXT_DESC.fgColor
      });
}


