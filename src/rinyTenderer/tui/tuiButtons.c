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
    FramePos    mouseFramePos = tuiMapMouseToFrame(FRAME, mousePos);

    FramePos renderedTopLeft =
      {
        .x = (float)(int)BUTTON_DESC.topLeft.x,
        .y = (float)(((int)BUTTON_DESC.topLeft.y / 2.0f) * 2),
        .z = BUTTON_DESC.topLeft.z
      };
    FramePos renderedBottomRight =
      {
        .x = (float)(int)BUTTON_DESC.bottomRight.x,
        .y = (float)(((int)BUTTON_DESC.bottomRight.y / 2.0f) * 2),
        .z = BUTTON_DESC.bottomRight.z
      };
    if (tuiIsPointInBounds(mouseFramePos, renderedTopLeft, renderedBottomRight))
    {
      clicked = true;
    }
  }

 // - - - Fill every cell touched by the button, including border cells.
  int startCellX = (int)BUTTON_DESC.topLeft.x;
  int endCellX   = (int)BUTTON_DESC.bottomRight.x;
  int startCellY = (int)BUTTON_DESC.topLeft.y / 2;
  int endCellY   = (int)BUTTON_DESC.bottomRight.y / 2;

  for (uint16_t cy = startCellY; cy <= endCellY; ++cy)
  {
    for (uint16_t cx = startCellX; cx <= endCellX; ++cx)
    {
      frameSetChar(
        FRAME,
        (FramePos){ .x = (float)cx, .y = (float)(cy * 2), .z = BUTTON_DESC.topLeft.z },
        " ",
        BUTTON_DESC.fillColor,
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
  float   boxWidth         = BUTTON_DESC.bottomRight.x - BUTTON_DESC.topLeft.x;
  size_t  availableWidth    = (boxWidth >= 4.0f) ? (size_t)(boxWidth - 4.0f) : 0;

  // - - - Draw centered text label inside the button box (snapped to even sub-pixel row)
  float height              = BUTTON_DESC.bottomRight.y - BUTTON_DESC.topLeft.y;
  float rawCenterY          = BUTTON_DESC.topLeft.y + (height / 2.0f);
  float centerY             = (float)(((int)rawCenterY / 2.0f) * 2);

  TuiTextCentered textDesc  = 
    {
      .position = 
        { 
          .x = BUTTON_DESC.topLeft.x + 2.0f, 
          .y = centerY, 
          .z = BUTTON_DESC.topLeft.z - 2.0f 
        },
      .width    = availableWidth,
      .text     = BUTTON_DESC.text,
      .maxLen   = BUTTON_DESC.maxLen,
      .fgColor  = BUTTON_DESC.accentColor,
      .bgColor  = TERMINAL_COLOR_TRANSPARENT
    };

  tuiDrawTextCentered(FRAME, textDesc);

  return clicked;
}
