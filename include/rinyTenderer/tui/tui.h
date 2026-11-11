#pragma once

#include "justLibrary.h"
#include "rinyTenderer/_structs.h"
#include <rinyTenderer/tui/widgets.h>


// - - - TUI Box & Panel Drawing - - -

JUST_API void tuiDrawBox(TerminalFrame* FRAME, TuiBox BOX_DESC);


// - - - TUI Text & Label Rendering - - -

JUST_API void tuiDrawText(TerminalFrame* FRAME, TuiText TEXT_DESC);

JUST_API void tuiDrawTextCentered(TerminalFrame* FRAME, TuiTextCentered TEXT_DESC);


// - - - TUI Widgets & Meters - - -

JUST_API void tuiDrawProgressBar(TerminalFrame* FRAME, TuiProgressBar PROGRESS_DESC);

JUST_API bool tuiButton(TerminalFrame* FRAME, const TuiButton BUTTON_DESC);

JUST_API bool tuiCheckbox(TerminalFrame* FRAME, const TuiCheckbox CHECK_DESC);

JUST_API void tuiSlider(TerminalFrame* FRAME, const TuiSlider DESC);

JUST_API static inline bool tuiIsPointInBounds(FramePos POINT, FramePos TOP_LEFT, FramePos BOTTOM_RIGHT)
{
  return (POINT.x >= TOP_LEFT.x && POINT.x <= BOTTOM_RIGHT.x &&
          POINT.y >= TOP_LEFT.y && POINT.y <= BOTTOM_RIGHT.y);
}

JUST_API static inline FramePos tuiMapMouseToFrame(const TerminalFrame* FRAME, TerminalPos mousePos)
{
  // - - - Convert 1-indexed terminal screen cells to 0-indexed frame-relative sub-pixels
  float relX = (float)mousePos.x - 1.0f - FRAME->position.x;
  float relY = ((float)mousePos.y - 1.0f - FRAME->position.y) * 2.0f;

  return (FramePos)
  {
    .x = relX,
    .y = relY,
    .z = 0.0f
  };
}
