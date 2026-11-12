#include "justLibrary.h"
#include "rinyTenderer/_structs.h"
#include <rinyTenderer/tui/tui.h>
#include <rinyTenderer/renderer/frame.h>
#include <stdint.h>

// - - - TUI Box and Panel Drawing - - -

JUST_API void tuiDrawBox(TerminalFrame* FRAME, const TuiBox BOX_DESC)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);

  if (BOX_DESC.topLeft.x >= BOX_DESC.bottomRight.x) return;
  if (BOX_DESC.topLeft.y >= BOX_DESC.bottomRight.y) return;

  const char* tl;
  const char* tr;
  const char* bl;
  const char* br;
  const char* h;
  const char* v;

  switch(BOX_DESC.boxType)
  {
    case TUI_BORDER_NONE: return;

    case TUI_BORDER_ROUNDED:
      tl = "╭";
      bl = "╰";
      tr = "╮";
      br = "╯";
      h  = "─";
      v  = "│";
      break;

    case TUI_BORDER_SHARP  :
      tl = "┌";
      bl = "└";
      tr = "┐";
      br = "┘";
      h  = "─";
      v  = "│";
      break;

    case TUI_BORDER_HEAVY  :
      tl = "┏";
      bl = "┗";
      tr = "┓";
      br = "┛";
      h  = "━";
      v  = "┃";
      break;

    case TUI_BORDER_DOUBLE :
      tl = "╔";
      bl = "╚";
      tr = "╗";
      br = "╝";
      h  = "═";
      v  = "║";
      break;

    case TUI_BORDER_ASCII  :
      tl = "+";
      bl = "+";
      tr = "+";
      br = "+";
      h  = "─";
      v  = "│";
      break;
  }

  // - - - Corners
  frameSetChar(FRAME, BOX_DESC.topLeft, tl, BOX_DESC.color, TERMINAL_COLOR_TRANSPARENT);
  frameSetChar(FRAME, BOX_DESC.bottomRight, br, BOX_DESC.color, TERMINAL_COLOR_TRANSPARENT);
  frameSetChar(
    FRAME, 
    (FramePos)
    {
      .x = BOX_DESC.topLeft.x, 
      .y = BOX_DESC.bottomRight.y, 
      .z = (BOX_DESC.topLeft.z + BOX_DESC.bottomRight.z) / 2
    }, 
    bl, 
    BOX_DESC.color, 
    TERMINAL_COLOR_TRANSPARENT);
  frameSetChar(
    FRAME, 
    (FramePos)
    {
      .x = BOX_DESC.bottomRight.x, 
      .y = BOX_DESC.topLeft.y, 
      .z = (BOX_DESC.topLeft.z + BOX_DESC.bottomRight.z) / 2
    }, 
    tr, 
    BOX_DESC.color, 
    TERMINAL_COLOR_TRANSPARENT);

  // - - - Horizontal Edges
  for (uint16_t x = BOX_DESC.topLeft.x + 1; x < BOX_DESC.bottomRight.x; ++x)
  {
    float progress = (x - BOX_DESC.topLeft.x) / (BOX_DESC.bottomRight.x - BOX_DESC.topLeft.x);
    float z        = ((1 - progress) * BOX_DESC.topLeft.z) + (progress * BOX_DESC.bottomRight.z);

    frameSetChar(
      FRAME, 
      (FramePos)
      {
        .x = x,
        .y = BOX_DESC.topLeft.y,
        .z = z
      },
      h,
      BOX_DESC.color,
      TERMINAL_COLOR_TRANSPARENT);
    frameSetChar(
      FRAME, 
      (FramePos)
      {
        .x = x,
        .y = BOX_DESC.bottomRight.y,
        .z = z
      },
      h,
      BOX_DESC.color,
      TERMINAL_COLOR_TRANSPARENT);
  }

  // - - - Vertical edges
  for (uint16_t y = BOX_DESC.topLeft.y + 2; y < BOX_DESC.bottomRight.y; y+= 2)
  {
    float progress = (y - BOX_DESC.topLeft.y) / (BOX_DESC.bottomRight.y - BOX_DESC.topLeft.y);
    float z        = ((1 - progress) * BOX_DESC.topLeft.z) + (progress * BOX_DESC.bottomRight.z);

    frameSetChar(
      FRAME, 
      (FramePos)
      {
        .x = BOX_DESC.topLeft.x,
        .y = y,
        .z = z
      },
      v,
      BOX_DESC.color,
      TERMINAL_COLOR_TRANSPARENT);
    frameSetChar(
      FRAME, 
      (FramePos)
      {
        .x = BOX_DESC.bottomRight.x,
        .y = y,
        .z = z
      },
      v,
      BOX_DESC.color,
      TERMINAL_COLOR_TRANSPARENT);
  }
}
