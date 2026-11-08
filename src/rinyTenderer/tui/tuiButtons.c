#include "rinyTenderer/window.h"
#include <rinyTenderer/tui/tui.h>
#include <rinyTenderer/renderer/frame.h>

JUST_API bool tuiButton(TerminalFrame* FRAME, const TuiButton BUTTON_DESC)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG(BUTTON_DESC.text != NULL);

  if (BUTTON_DESC.topLeft.x >= BUTTON_DESC.bottomRight.x ||
      BUTTON_DESC.topLeft.y >= BUTTON_DESC.bottomRight.y)
  {
    return false;
  }

  // - - - Hit-Test mouse position against button bounds
  bool clicked = false;
  if (terminalIsMouseButtonDown(MOUSE_BUTTON_LEFT) || terminalIsKeyDown(KEY_ENTER))
  {
    TerminalPos mousePos      = terminalGetMousePos();
    FramePos    mouseFramePos = 
      { 
        .x = mousePos.x, 
        .y = mousePos.y * 2.0f - 2.0f, 
        .z = 0.0f 
      };
    
    if (tuiIsPointInBounds(mouseFramePos, BUTTON_DESC.topLeft, BUTTON_DESC.bottomRight))
    {
      clicked = true;
    }
  }

  // - - - Fill inner background area
// - - - Fill inner background area (Cell-based row/col iteration)
  uint16_t startCellX = (uint16_t)BUTTON_DESC.topLeft.x + 1;
  uint16_t endCellX   = (uint16_t)BUTTON_DESC.bottomRight.x;
  uint16_t startCellY = ((uint16_t)BUTTON_DESC.topLeft.y / 2) + 1;
  uint16_t endCellY   = (uint16_t)BUTTON_DESC.bottomRight.y / 2;

  for (uint16_t cy = startCellY; cy < endCellY; ++cy)
  {
    for (uint16_t cx = startCellX; cx < endCellX; ++cx)
    {
      frameSetChar(
        FRAME,
        (FramePos){ .x = (float)cx, .y = (float)(cy * 2), .z = BUTTON_DESC.topLeft.z },
        " ",
        TERMINAL_COLOR_TRANSPARENT,
        BUTTON_DESC.fillColor
      );
    }
  }

  // - - - Draw outer border box
  tuiDrawBox(FRAME, 
      (TuiBox)
      {
        .topLeft     = BUTTON_DESC.topLeft,
        .bottomRight = BUTTON_DESC.bottomRight,
        .boxType     = BUTTON_DESC.borderType,
        .color       = BUTTON_DESC.accentColor
      });

  // - - - Calculate available text width and clamp maxLen
  float   boxWidth          = BUTTON_DESC.bottomRight.x - BUTTON_DESC.topLeft.x;
  size_t  availableWidth    = (boxWidth >= 4.0f) ? (size_t)(boxWidth - 4.0f) : 0;
  size_t  effectiveMaxLen   = BUTTON_DESC.maxLen;
  if (effectiveMaxLen > availableWidth)
  {
    effectiveMaxLen = availableWidth;
  }

  // - - - Draw centered text label inside the button box
  if (effectiveMaxLen > 0)
  {
    float height              = BUTTON_DESC.bottomRight.y - BUTTON_DESC.topLeft.y;
    float centerY             = BUTTON_DESC.topLeft.y + (height / 2.0f);
    TuiTextCentered textDesc  = 
      {
        .position = 
          { 
            .x = BUTTON_DESC.topLeft.x + 2.0f, 
            .y = centerY, 
            .z = BUTTON_DESC.topLeft.z - 2.0f 
          },
        .width    = (size_t)(boxWidth - 4.0f),
        .text     = BUTTON_DESC.text,
        .maxLen   = effectiveMaxLen,
        .fgColor  = BUTTON_DESC.accentColor,
        .bgColor  = TERMINAL_COLOR_TRANSPARENT
      };

    tuiDrawTextCentered(FRAME, textDesc);
  }

  return clicked;
}
