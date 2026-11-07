#include "justLibrary.h"
#include "rinyTenderer/_structs.h"
#include <rinyTenderer/tui/tui.h>
#include <rinyTenderer/renderer/frame.h>
#include <stdint.h>

// - - - TUI Box and Panel Drawing - - -

JUST_API void tuiDrawBox(
    TerminalFrame*  FRAME, 
    FramePos        TOP_LEFT,
    FramePos        BOTTOM_RIGHT,
    TerminalColor   COLOR,
    TuiBoxType      BOX_TYPE)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);

  if (TOP_LEFT.x >= BOTTOM_RIGHT.x) return;
  if (TOP_LEFT.y >= BOTTOM_RIGHT.y) return;

  const char* tl;
  const char* tr;
  const char* bl;
  const char* br;
  const char* h;
  const char* v;

  switch(BOX_TYPE)
  {
    case ROUNDED:
      tl = "╭";
      bl = "╰";
      tr = "╮";
      br = "╯";
      h  = "─";
      v  = "│";
      break;

    case SHARP  :
      tl = "┌";
      bl = "└";
      tr = "┐";
      br = "┘";
      h  = "─";
      v  = "│";
      break;

    case HEAVY  :
      tl = "┏";
      bl = "┗";
      tr = "┓";
      br = "┛";
      h  = "━";
      v  = "┃";
      break;

    case DOUBLE :
      tl = "╔";
      bl = "╚";
      tr = "╗";
      br = "╝";
      h  = "═";
      v  = "║";
      break;

    case ASCII  :
      tl = "+";
      bl = "+";
      tr = "+";
      br = "+";
      h  = "─";
      v  = "│";
      break;
  }

  // - - - Corners
  frameSetChar(FRAME, TOP_LEFT, tl, COLOR, TERMINAL_COLOR_TRANSPARENT);
  frameSetChar(FRAME, BOTTOM_RIGHT, br, COLOR, TERMINAL_COLOR_TRANSPARENT);
  frameSetChar(
      FRAME, 
      (FramePos)
      {
        .x = TOP_LEFT.x, 
        .y = BOTTOM_RIGHT.y, 
        .z = (TOP_LEFT.z + BOTTOM_RIGHT.z) / 2
      }, 
      bl, 
      COLOR, 
      TERMINAL_COLOR_TRANSPARENT);
  frameSetChar(
      FRAME, 
      (FramePos)
      {
        .x = BOTTOM_RIGHT.x, 
        .y = TOP_LEFT.y, 
        .z = (TOP_LEFT.z + BOTTOM_RIGHT.z) / 2
      }, 
      tr, 
      COLOR, 
      TERMINAL_COLOR_TRANSPARENT);

  // - - - Horizontal Edges
  for (uint16_t x = TOP_LEFT.x + 1; x < BOTTOM_RIGHT.x; ++x)
  {
    float progress = (x - TOP_LEFT.x) / (BOTTOM_RIGHT.x - TOP_LEFT.x);
    float z        = ((1 - progress) * TOP_LEFT.z) + (progress * BOTTOM_RIGHT.z);

    frameSetChar(
        FRAME, 
        (FramePos)
        {
          .x = x,
          .y = TOP_LEFT.y,
          .z = z
        },
        h,
        COLOR,
        TERMINAL_COLOR_TRANSPARENT);
    frameSetChar(
        FRAME, 
        (FramePos)
        {
          .x = x,
          .y = BOTTOM_RIGHT.y,
          .z = z
        },
        h,
        COLOR,
        TERMINAL_COLOR_TRANSPARENT);
  }

  // - - - Vertical edges
  for (uint16_t y = TOP_LEFT.y + 1; y < BOTTOM_RIGHT.y; y++)
  {
    float progress = (y - TOP_LEFT.y) / (BOTTOM_RIGHT.y - TOP_LEFT.y);
    float z        = ((1 - progress) * TOP_LEFT.z) + (progress * BOTTOM_RIGHT.z);

    frameSetChar(
        FRAME, 
        (FramePos)
        {
          .x = TOP_LEFT.x,
          .y = y,
          .z = z
        },
        v,
        COLOR,
        TERMINAL_COLOR_TRANSPARENT);
    frameSetChar(
        FRAME, 
        (FramePos)
        {
          .x = BOTTOM_RIGHT.x,
          .y = y,
          .z = z
        },
        v,
        COLOR,
        TERMINAL_COLOR_TRANSPARENT);
  }
}

JUST_API void tuiFillRect(
    TerminalFrame*  FRAME,
    FramePos        TOP_LEFT,
    FramePos        BOTTOM_RIGHT,
    const char*     CHARACTER,
    TerminalColor   FG_COLOR,
    TerminalColor   BG_COLOR)
{
  JUST_ASSERT_DEBUG(FRAME);

  if (TOP_LEFT.x >= BOTTOM_RIGHT.x) return;
  if (TOP_LEFT.y >= BOTTOM_RIGHT.y) return;

  for (uint16_t y = TOP_LEFT.y; y <= BOTTOM_RIGHT.y; y++)
  {
    for (uint16_t x = TOP_LEFT.x; x <= BOTTOM_RIGHT.x; x++)
    {
      float progressY = (y - TOP_LEFT.y) / (BOTTOM_RIGHT.y - TOP_LEFT.y);
      float progressX = (x - TOP_LEFT.x) / (BOTTOM_RIGHT.x - TOP_LEFT.x);
      float progress  = (progressY + progressX) / 2;
      float z         = ((1 - progress) * TOP_LEFT.z) + (progress * BOTTOM_RIGHT.z);
      frameSetChar(
          FRAME, 
          (FramePos)
          {
            .x = x,
            .y = y,
            .z = z
          }, 
          CHARACTER, 
          FG_COLOR, 
          BG_COLOR);
    }
  }
}
