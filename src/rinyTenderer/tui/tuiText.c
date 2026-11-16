#include "justLibrary.h"
#include "rinyTenderer/_structs.h"
#include "rinyTenderer/window.h"
#include <rinyTenderer/tui/tui.h>
#include <rinyTenderer/renderer/frame.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

JUST_API void tuiDrawText(TerminalFrame* FRAME, TuiText TEXT_DESC)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG(TEXT_DESC.text != NULL);

  float startX = TEXT_DESC.position.x;
  float curX   = startX;
  float curY   = TEXT_DESC.position.y;
  float curZ   = TEXT_DESC.position.z;

  size_t length = strnlen(TEXT_DESC.text, TEXT_DESC.maxLen);
  for (size_t i = 0; i < length; ++i)
  {
    if (TEXT_DESC.text[i] == '\n')
    {
      curX = startX;
      curY += 2.0f;
      continue;
    }

    // - - - Auto-wrap if width is constrained and we've reached the boundary
    if (TEXT_DESC.width > 0 && (curX - startX) >= (float)TEXT_DESC.width)
    {
      curX = startX;
      curY += 2.0f;
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
      .width    = TEXT_DESC.width,
      .position = centeredPos,
      .bgColor  = TEXT_DESC.bgColor,
      .fgColor  = TEXT_DESC.fgColor
    });
}

JUST_API bool tuiDrawTextInput(TerminalFrame* FRAME, TuiTextInput TEXT_INPUT)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG(TEXT_INPUT.buffer != NULL);
  JUST_ASSERT_DEBUG(TEXT_INPUT.bufferSize > 0);

  // - - - Snap Y to even sub-pixel cell row boundary
  int       cellY   = (int) TEXT_INPUT.topLeft.y / 2;
  FramePos  topLeft = 
    {
      .x = TEXT_INPUT.topLeft.x,
      .y = cellY * 2,
      .z = TEXT_INPUT.topLeft.z
    };
  cellY                 = (int) TEXT_INPUT.bottomRight.y / 2;
  FramePos bottomRight  = 
    {
      .x = TEXT_INPUT.bottomRight.x,
      .y = cellY * 2,
      .z = TEXT_INPUT.bottomRight.z
    };

  // - - - Focus Management via Mouse Click
  static char* focusedInputBuffer = NULL;
  if (terminalIsMouseButtonDown(MOUSE_BUTTON_LEFT))
  {
    TerminalPos mousePos      = terminalGetMousePos();
    FramePos    mouseFramePos = tuiMapMouseToFrame(FRAME, mousePos);

    if (tuiIsPointInBounds(mouseFramePos, topLeft, bottomRight))
    { focusedInputBuffer = TEXT_INPUT.buffer; }
    else if (focusedInputBuffer == TEXT_INPUT.buffer)
    { focusedInputBuffer = NULL; }
  }

  bool isFocused  = (focusedInputBuffer == TEXT_INPUT.buffer);
  bool modified   = false;

  // - - - Handle Keyboard input if focused
  size_t      currLen   = strlen(TEXT_INPUT.buffer);
  if (isFocused)
  {
    size_t      typedLen  = terminalGetTextLength();
    const char* typedText = terminalGetText();

    // - - - Handle Backspace
    if (terminalIsKeyDown(KEY_BACKSPACE) && currLen > 0)
    {
      TEXT_INPUT.buffer[currLen - 1] = '\0';
      modified = true;
    }

    if (typedLen > 0 && typedText != NULL)
    {
      for (size_t i = 0; i < typedLen; ++i)
      {
        if (currLen + 1 < TEXT_INPUT.bufferSize)
        {
          char c                          = typedText[i];
          TEXT_INPUT.buffer[currLen]      = c;
          TEXT_INPUT.buffer[currLen + 1]  = '\0';
          modified                        = true;
          currLen++;
        }
      }
    }
  }

  // - - - Render text buffer
  tuiDrawText(FRAME, (TuiText)
    {
      .position = topLeft,
      .text     = TEXT_INPUT.buffer,
      .maxLen   = TEXT_INPUT.bufferSize,
      .width    = TEXT_INPUT.width,
      .fgColor  = TEXT_INPUT.fgColor,
      .bgColor  = TEXT_INPUT.bgColor
    });

  // - - - Render Cursor mathematically accounting for wrapping
  if (isFocused)
  {
    size_t lineIdx = currLen / (size_t)TEXT_INPUT.width;
    size_t colIdx  = currLen % (size_t)TEXT_INPUT.width;

    FramePos cursorPos = 
      {
        .x = topLeft.x + (float)colIdx,
        .y = topLeft.y + (float)(lineIdx * 2),
        .z = topLeft.z
      };

    static uint8_t blinkTimer = 0;
    blinkTimer++;
    bool showCursor = (blinkTimer < 64);
    blinkTimer = blinkTimer % 96;

    if (showCursor)
    {
      frameSetChar(FRAME, cursorPos, "█", TEXT_INPUT.cursorColor, TEXT_INPUT.bgColor);
    }
  }

  return modified;
}
