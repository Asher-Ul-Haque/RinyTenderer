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



JUST_API static inline bool tuiIsPointInBounds(FramePos POINT, FramePos TOP_LEFT, FramePos BOTTOM_RIGHT)
{
  return (POINT.x >= TOP_LEFT.x && POINT.x <= BOTTOM_RIGHT.x &&
          POINT.y >= TOP_LEFT.y && POINT.y <= BOTTOM_RIGHT.y);
}
